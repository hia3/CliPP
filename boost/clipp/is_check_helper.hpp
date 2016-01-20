// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_IS_CHECK_HELPER_HPP_HOLT_28092003
#define BOOST_CLIPP_IS_CHECK_HELPER_HPP_HOLT_28092003
#include <boost/config.hpp>
#if BOOST_WORKAROUND( BOOST_MSVC, > 1900)

template<template<typename...> class T>
struct is_template_of_impl
{
    template<typename Other>
    struct result : std::false_type {};
    template<typename... As>
    struct result<T<As...> > : std::true_type {};
};

template<template<typename...> class T, typename Inst>
constexpr auto is_template_of = is_template_of_impl<T>::template result<Inst>::value;

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
