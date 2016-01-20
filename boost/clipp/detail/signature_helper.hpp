// Copyright Peder Holt 2003-2004. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_SIGNATURE_HELPER_HPP_HOLT_28022004
#define BOOST_CLIPP_SIGNATURE_HELPER_HPP_HOLT_28022004

#include <boost/clipp/config.hpp>
#include <boost/clipp/detail/cv_category.hpp>
#include <boost/type.hpp>

#include <boost/mpl/at.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/minus.hpp>
#include <boost/mpl/size.hpp>

namespace boost { namespace clipp { namespace detail {

//Type tags
struct free_function_tag {};
struct free_data_tag {};
struct constructor_tag {};
struct member_function_tag {};
struct member_data_tag {};

template<typename Signature>
struct signature_arity
{
    BOOST_STATIC_CONSTANT(unsigned,value=(mpl::minus<mpl::size<Signature>,mpl::int_<4> >::value));
};

template<typename Signature>
struct signature_type
{
    typedef typename mpl::at<Signature,mpl::int_<0> >::type type;
};

//Return type selector
template<typename T>
struct return_type_selector
{
    typedef typename boost::type<T> type;
};

template<>
struct return_type_selector<void>
{
    typedef mpl::void_ type;
};

template<typename Signature>
struct return_type_wrapper
{
    typedef typename return_type_selector<typename mpl::at<Signature,mpl::int_<1> >::type>::type type;
};

template<typename Signature>
struct return_type
{
    typedef typename mpl::at<Signature,mpl::int_<1> >::type type;
};

template<typename Signature>
struct data_type
{
    typedef typename mpl::at<Signature,mpl::int_<2> >::type type;
};

//CV Tag type
template<typename Signature>
struct cv_type
{
    typedef typename mpl::at<Signature,mpl::int_<3> >::type type;
};

}}} //namespace boost::clipp::detail

#endif //BOOST_CLIPP_SIGNATURE_HELPER_HPP_HOLT_28022004
