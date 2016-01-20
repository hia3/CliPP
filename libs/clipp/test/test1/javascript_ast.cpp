#include <boost/call_traits.hpp>
#include <iterator>
#include <set>
#include <boost/type_traits/detail/yes_no_type.hpp>

//#include <boost/clipp/member_function.hpp>
/*#include <boost/clipp/define_read.hpp>
#include <boost/clipp/define_constructor.hpp>
#include <boost/clipp/define_write.hpp>
#include <boost/clipp/define_operator.hpp>

#include <boost/clipp/context.hpp>
#include <typeinfo>
*/
/*
namespace boost {
template <typename T>
struct is_const_pointer_helper
{
    static type_traits::no_type BOOST_TT_DECL _m_check(...);
    template<typename T>
    static type_traits::yes_type BOOST_TT_DECL _m_check(T const& (*)());

    typedef typename add_reference<T>::type (*fn)();

    BOOST_STATIC_CONSTANT(bool, value =
        sizeof(_m_check(((fn)0)) ) == sizeof(type_traits::yes_type)
    );
};                    

template<typename T>
struct is_const_pointer_base {
    template<bool value>
    struct inner : mpl::bool_<is_const_pointer_helper<T>::value>
    {};
    template<>
    struct inner<false> : mpl::false_
    {};
};

template<typename T>
struct is_const_pointer :
is_const_pointer_base<T>::inner<is_pointer<T>::value>
{};
}

Need to support:
T
T const
T &
T const&
T *
T const*
T * const
T const * const
T *&
T const*&
T * const&
T const * const&

//int *const -> pointer is const
//int const* -> pointed to is const

int
    all

int const
    all

int&
    int&
    int*
    int*const

int const&
    all

int*
    int&
    int*
    int*const

int const*
    all

int *const
    int&
    int*
    int*const

int const*const
    int&
    int*
    int*const

*/

struct class_test {
    int a;
    int b;
    int c;
};

void type_test(class_test const * t) {
    int a= 0;
}

template<typename T>
void type_test(T& a,T& b) {
    if(&a==&b) {
        //Reference
        int c=0;
    }
    else {
        int c=1;
    }
}

template<typename T>
void type_test(T* a,T* b) {
    if(&a==&b) {
        //Reference
        int c=0;
    }
    else {
        int c=1;
    }
}

class_test g_test;

class_test const& ret_type() {
    return g_test;
}
void test(int const*const a) {
    int b=0;
}

int test_2(int a) {return a;}
struct test_c {
    test_c(int) {}
    int b;
    int c_1(int a) {return 4;}
    int c_2() const {return 3;}
    static char d(test_c& test) {return 1;}
    static double e() {return 2;}
    test_c operator-(int a) const {return test_c(a);}
};
/*
namespace boost {namespace clipp {
template<typename C>
class class2_ 
:   public detail::define_constructor<C,class2_>
,   public detail::define_function<C,class2_>
,   public detail::define_read<C,class2_>
,   public detail::define_write<C,class2_>
,   public detail::define_operator<C,class2_>
{
public:
    template<typename MemberType,typename T>
    MemberType& define_member(const char* name,T value,boost::type<MemberType>&) {
        MemberType* member=new MemberType(name,value);
        return *member;
    }
};
}}//namespace boost::clipp*/
namespace boost {
namespace detail {
template<typename T>
struct is_iterator_helper {
    static type_traits::no_type BOOST_TT_DECL _m_check(...);
    template<class A0,typename A1,typename A2>
    static type_traits::yes_type BOOST_TT_DECL _m_check(std::iterator<A0,A1,A2> const& (*)());    
    template<class A0,typename A1,typename A2>
    static type_traits::yes_type BOOST_TT_DECL _m_check(std::iterator<A0,A1,A2> const* (*)());    
    template<class A0,typename A1,typename A2>
    static type_traits::yes_type BOOST_TT_DECL _m_check(std::iterator<A0,A1,A2> & (*)());    
    template<class A0,typename A1,typename A2>
    static type_traits::yes_type BOOST_TT_DECL _m_check(std::iterator<A0,A1,A2> * (*)());    
    template<class A0,typename A1,typename A2>
    static type_traits::yes_type BOOST_TT_DECL _m_check(std::iterator<A0,A1,A2> (*)());    
    template<class A0,typename A1,typename A2>
    static type_traits::yes_type BOOST_TT_DECL _m_check(std::iterator<A0,A1,A2> const (*)());    

    typedef typename T (*fn)();                                                          

    BOOST_STATIC_CONSTANT(bool, value = 
        sizeof(_m_check(((fn)0)) ) == sizeof(type_traits::yes_type)
    );
};

} //namespace detail


template<typename T>
struct is_iterator : public
    mpl::bool_<detail::is_iterator_helper<T>::value>
{};
}
std::set<int> test_map;

std::set<int> const& get_test_map() {
    return test_map;
}

template<typename A0,typename A1,typename A2>
void iterator_tester(std::iterator<A0,A1,A2>& iterator) {
}

/*
template<typename C>
struct class_ 
:   public boost::clipp::detail::define_read<C,class_>
{
    template<typename T>
    void insert_member(const char* name,T* member) {}
};*/


void hei(int) {}
void hopp(int) {}
void main() {
    iterator_tester(get_test_map().begin());
/*    boost::clipp::class2_<test_c> cls;
    std::string text = cls.function("MyFunction",test_c::c_1).signature("arg1").format();
    cls.constructor(boost::clipp::arguments<int>());
    cls.write("b",&test_c::b);
    cls.def_operator(boost::clipp::self - int());*/

/*    class_<test_c> cls;
    cls.read("b",&test_c::b);
    cls.read("c",8.65);
    cls.read("c1",test_c::c_1);
    cls.read("c2",test_c::c_2);
    cls.read("d",test_c::d);
    cls.read("e",test_c::e);*/
    hei(5),hopp(6);

/*    typedef boost::mpl::list<double,boost::clipp::detail::member_function_tag,boost::clipp::detail::const_,test_c,int,char*,const test_c&> signature;
    typedef boost::mpl::list<char,boost::clipp::detail::free_data_tag,boost::clipp::detail::cv_unqualified> signature;
    typedef boost::clipp::detail::signature_type<signature>::type type;
    typedef boost::clipp::detail::signature_type<signature>::type_tag type_tag;
    typedef boost::clipp::detail::signature_type<signature>::cv_tag cv_tag;
    const char* type_tag_name=typeid(type_tag).name();
    const char* cv_tag_name=typeid(cv_tag).name();
    int arity=boost::clipp::detail::signature_type<signature>::arity;
    const char* type_name=typeid(type).name();*/
    type_test(ret_type(),ret_type());
    int a=1;
    int const a_c=a;
    int& a_ref=a;
    int const& a_cref=a;
    int* a_ptr=&a;
    int const* a_cptr=&a;
    int *const a_ptrc=&a;
    int const*const a_cptrc=&a;
    test(&a);
    test(&a_c);
    test(&a_ref);
    test(&a_cref);
    test(a_ptr);
    test(a_cptr);
    test(a_ptrc);
    test(a_cptrc);
}
