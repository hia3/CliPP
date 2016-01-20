#include <boost/clipp.hpp>
#include <boost/javascript.hpp>

using namespace boost::clipp;
using namespace boost::javascript;

class test : public value {
public:
    static void init(context* c);
    void fn_ptr(int,long) {}
    int fn_ptr(int) {return 1;}
    int fn_ptr2(void) {return 0;}
    int operator+(int) const {return 0;}
};

void test::init(context* c)
{
    class_<test> cls("name",c);
    //Short syntaxes
    cls[constructor<>()];
    //Function overloads, need to supply function signature.
    cls["fn"]   =   function<int (test::*)(int)>(&test::fn_ptr);
    cls["fn2"]  =   &test::fn_ptr2;

    cls[self+int()];
    //Long syntaxes
/*    cls.read("x",fn_ptr1);
    cls.write("x",fn_ptr2);
    cls.read_write("x",fn_ptr1,fn_ptr2);
    cls.function("fn",fn_ptr);
    cls.static_function("fn",fn_ptr);
    cls.constructor(arguments<...>());*/
}

int main() {
    javascript_parser parser;
    test::init(parser.get_context());
    return 0;
}
