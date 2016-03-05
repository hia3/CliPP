#if defined(__clang__) && defined(_MSC_VER)
#define BOOST_LIB_TOOLSET "vc140" // Remove this if you build Boost on Windows with clang
#endif

#include <boost/clipp.hpp>
#include <boost/javascript.hpp>

#include <boost/coroutine2/all.hpp>
#include <boost/chrono/chrono.hpp>
#include <boost/scope_exit.hpp>

#include <iostream>
#include <queue>


using coro_void_0 = boost::coroutines2::coroutine<void>;

// This can be thread_local (every thread has to call 'run' function) 
// or lockfree queue from Boost.Lockfree (only main thread calls 'run' function)
// This example has only one thread
thread_local std::queue<coro_void_0::pull_type *> coroutine_queue;

// Add a non-finished coroutines to a queue to run it later
void post_coroutine(coro_void_0::pull_type * coro)
{
    coroutine_queue.push(coro);
}

// Get a coroutine (if any) from the queue
coro_void_0::pull_type * get_coroutine()
{
    if (coroutine_queue.empty())
    {
        return nullptr;
    }

    coro_void_0::pull_type * result = coroutine_queue.front(); 
    coroutine_queue.pop();
    return result;
}

// Call a coroutine, put it back to the queue if it is not finished
void call_coroutine(coro_void_0::pull_type * coro)
{
    if (coro)
    {
        (*coro)();

        if (*coro)
        {
            post_coroutine(coro);
        }
        else
        {
            delete coro;
        }
    }
}

// Round-robin coroutines (until application is not closed, for example)
void run(std::function<bool()> predicate)
{
    while (predicate())
    {
        if (auto c = get_coroutine())
        {
            call_coroutine(c);
        }
        else
        {
            return;
        }
    }

    // TODO: use unique_ptr instead of raw pointers, call coroutine_queue.clear() here.
    while (auto coroutine = get_coroutine())
    {
        delete coroutine;
    }
    assert(!get_coroutine());
}

// Run f until it yields, then put it to the queue
// If f finishes immediately (without yielding), don't put it to the queue
template<typename F>
void spawn(F && f, std::size_t stack_size = 0x100000)
{
    boost::coroutines2::protected_fixedsize_stack stack(stack_size);
    auto coroutine = std::make_unique<coro_void_0::pull_type>(stack, std::forward<F>(f), true);
    if (*coroutine)
    {
        post_coroutine(coroutine.release());
    }
}

// branch_call has to know the current coroutine to yield
// So every time we are going to call js, we store current coroutine (see javascript_coro_handler::push)
// We are using std::stack here because a js function can spawn another js function
struct javascript_coro_handler : boost::javascript::exception_handler
{
    javascript_coro_handler() {}

    javascript_coro_handler(javascript_coro_handler const & other) = delete;
    javascript_coro_handler & operator = (javascript_coro_handler const & right) = delete;

private:
    friend class scoped_yield;

    using push_t = coro_void_0::push_type *;

    void push(push_t y)
    {
        s.emplace(y);
    }

    void pop(push_t y)
    {
        assert(s.top() == y);
        s.pop();
    }

    void branch_call() const override
    {
        auto yield = s.top();
        // We pop here, because yielding might result in calling another coroutine that expects it's own yield to be on the top of the stack
        s.pop();

        BOOST_SCOPE_EXIT(&yield, this_)
        {
            this_->s.push(yield); // put it back so that next time they call us, we will have something to pop again
        }
        BOOST_SCOPE_EXIT_END

        // We don't have to yield here every single time. We can yield every 2 milliseconds, for example.
        (*yield)(); // this can throw forced_unwind exception, but stack is balanced using BOOST_SCOPE_EXIT
    }

    mutable std::stack<push_t> s;
};

// always use this class in a coroutine before passing control to the code that can call branch_call
class scoped_yield
{
    javascript_coro_handler & handler;
    coro_void_0::push_type & yield;
public:

    scoped_yield(javascript_coro_handler & handler, coro_void_0::push_type & yield) 
        : handler(handler), yield(yield)
    {
        handler.push(&yield);
    }
    ~scoped_yield()
    {
        handler.pop(&yield);
    }

    scoped_yield(scoped_yield const & other) = delete;
    scoped_yield & operator = (scoped_yield const & right) = delete;
};

struct MyClass
{
    MyClass(javascript_coro_handler & handler, boost::clipp::context * context) : handler(handler), context(context)
    {
    }

    MyClass(MyClass const & other) : handler(other.handler), context(other.context)
    {

    }
    MyClass & operator = (MyClass const &) = delete;

    void Echo(std::string const & line)
    {
        std::cout << line << std::endl;
    }

    // Can I use js_function instead of std::string as a parameter? std::function<void()>?
    // This has to be global function, but idk how to add a global functions easily
    void Spawn(std::string const & code)
    {
        auto value = context->lookup(code);

        if (value)
        {
            spawn
            (
                [this, value](coro_void_0::push_type & yield) mutable
                {
                    scoped_yield sy(handler, yield);
                    value();
                }
            );
        }
    }

private:
    javascript_coro_handler & handler;
    boost::clipp::context * context;
};

void start_main_js_fiber(javascript_coro_handler & handler, boost::javascript::javascript_parser & parser)
{
    spawn
    (
        [&](coro_void_0::push_type & yield)
        {
            std::ifstream script_file("spawn.js");

            scoped_yield sy(handler, yield);
            parser.parse(script_file); // If main fiber exits, we will leave this parse call, but all child js fibers will keep running

            std::cout << "Exiting main fiber" << std::endl;
        }
    );
}

int main(int argc, char * argv[])
{
    using namespace boost::clipp;
    using namespace boost::javascript;

    javascript_coro_handler handler;

    javascript_parser parser{ handler };
    auto context = parser.get_context();

    class_<MyClass> cls("MyClass", context);
    cls.function("Echo", &MyClass::Echo);
    cls.function("Spawn", &MyClass::Spawn);

    MyClass My(handler, context);
    context->global()->insert("My", wrap_ref(My, context));


    start_main_js_fiber(handler, parser);


    int iteration = 0;
    run([&iteration]() { /* process your GUI messages here */ iteration++; return iteration < 9000; });
}
