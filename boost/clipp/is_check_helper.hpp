// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_IS_CHECK_HELPER_HPP_HOLT_28092003
#define BOOST_CLIPP_IS_CHECK_HELPER_HPP_HOLT_28092003

#if BOOST_WORKAROUND( BOOST_MSVC, <= 1300)

#define BOOST_CLIPP_IS_1(name,expression)\
namespace detail {\
template<typename T>\
struct BOOST_PP_CAT(is_,name)_helper {\
    static type_traits::no_type BOOST_TT_DECL _m_check(...);\
    template<typename A0>\
    static type_traits::yes_type BOOST_TT_DECL _m_check(expression<A0> (*)());\
\
    typedef T (*fn)();\
\
    BOOST_STATIC_CONSTANT(bool, value = \
        sizeof(_m_check(((fn)0)) ) == sizeof(type_traits::yes_type)\
    );\
};\
}\
\
template<typename T>\
struct BOOST_PP_CAT(is_,name) : public\
    mpl::bool_<detail::BOOST_PP_CAT(is_,name)_helper<T>::value>\
{};

#define BOOST_CLIPP_IS_2(name,expression)\
namespace detail {\
template<typename T>\
struct BOOST_PP_CAT(is_,name)_helper {\
    static type_traits::no_type BOOST_TT_DECL _m_check(...);\
    template<typename A0,typename A1>\
    static type_traits::yes_type BOOST_TT_DECL _m_check(expression<A0,A1> (*)());\
\
    typedef T (*fn)();\
\
    BOOST_STATIC_CONSTANT(bool, value = \
        sizeof(_m_check(((fn)0)) ) == sizeof(type_traits::yes_type)\
    );\
};\
}\
\
template<typename T>\
struct BOOST_PP_CAT(is_,name) : public\
    mpl::bool_<detail::BOOST_PP_CAT(is_,name)_helper<T>::value>\
{};

#define BOOST_CLIPP_IS_3(name,expression)\
namespace detail {\
template<typename T>\
struct BOOST_PP_CAT(is_,name)_helper {\
    static type_traits::no_type BOOST_TT_DECL _m_check(...);\
    template<typename A0,typename A1,typename A2>\
    static type_traits::yes_type BOOST_TT_DECL _m_check(expression<A0,A1,A2> (*)());\
\
    typedef T (*fn)();\
\
    BOOST_STATIC_CONSTANT(bool, value = \
        sizeof(_m_check(((fn)0)) ) == sizeof(type_traits::yes_type)\
    );\
};\
}\
\
template<typename T>\
struct BOOST_PP_CAT(is_,name) : public\
    mpl::bool_<detail::BOOST_PP_CAT(is_,name)_helper<T>::value>\
{};

#define BOOST_CLIPP_IS_4(name,expression)\
namespace detail {\
template<typename T>\
struct BOOST_PP_CAT(is_,name)_helper {\
    static type_traits::no_type BOOST_TT_DECL _m_check(...);\
    template<typename A0,typename A1,typename A2,typename A3>\
    static type_traits::yes_type BOOST_TT_DECL _m_check(expression<A0,A1,A2,A3> (*)());\
\
    typedef T (*fn)();\
\
    BOOST_STATIC_CONSTANT(bool, value = \
        sizeof(_m_check(((fn)0)) ) == sizeof(type_traits::yes_type)\
    );\
};\
}\
\
template<typename T>\
struct BOOST_PP_CAT(is_,name) : public\
    mpl::bool_<detail::BOOST_PP_CAT(is_,name)_helper<T>::value>\
{};

#else 

#define BOOST_CLIPP_IS_1(name,expression)\
template<typename T>\
struct BOOST_PP_CAT(is_,name) : public mpl::false_ {};\
template<typename A0>\
struct BOOST_PP_CAT(is_,name)<expression<A0> > : public mpl::true_ {};

#define BOOST_CLIPP_IS_2(name,expression)\
template<typename T>\
struct BOOST_PP_CAT(is_,name) : public mpl::false_ {};\
template<typename A0,typename A1>\
struct BOOST_PP_CAT(is_,name)<expression<A0,A1> > : public mpl::true_ {};

#define BOOST_CLIPP_IS_3(name,expression)\
template<typename T>\
struct BOOST_PP_CAT(is_,name) : public mpl::false_ {};\
template<typename A0,typename A1,typename A2>\
struct BOOST_PP_CAT(is_,name)<expression<A0,A1,A2> > : public mpl::true_ {};

#define BOOST_CLIPP_IS_4(name,expression)\
template<typename T>\
struct BOOST_PP_CAT(is_,name) : public mpl::false_ {};\
template<typename A0,typename A1,typename A2,typename A3>\
struct BOOST_PP_CAT(is_,name)<expression<A0,A1,A2,A3> > : public mpl::true_ {};

#endif

#endif //BOOST_CLIPP_IS_CHECK_HELPER_HPP_HOLT_28092003
