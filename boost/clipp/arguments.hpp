// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_ARGUMENTS_HPP_HOLT_13082003
#define BOOST_CLIPP_ARGUMENTS_HPP_HOLT_13082003

#include "boost/clipp/config.hpp"
#include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
#include <boost/mpl/vector.hpp>
#include "boost/type_traits/detail/yes_no_type.hpp"

namespace boost { namespace clipp {

template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_CLIPP_MAX_ARITY, typename A,mpl::na)>
struct arguments : mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_CLIPP_MAX_ARITY, A)> {};

template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_CLIPP_MAX_ARITY, typename A,mpl::na)>
struct constructor : arguments<BOOST_PP_ENUM_PARAMS(BOOST_CLIPP_MAX_ARITY, A)> {};

template <typename T>
struct is_arguments_helper
{
    static boost::type_traits::no_type BOOST_TT_DECL _m_check(...);
    template<BOOST_PP_ENUM_PARAMS(BOOST_CLIPP_MAX_ARITY,typename A)>
    static boost::type_traits::yes_type BOOST_TT_DECL _m_check(int (*)(arguments<BOOST_PP_ENUM_PARAMS(BOOST_CLIPP_MAX_ARITY,A)>));

    static int fn(T);

    BOOST_STATIC_CONSTANT(bool, value = 
        sizeof( _m_check(fn) ) == sizeof(type_traits::yes_type)
    );
};

template<typename T>
struct is_arguments : public
    mpl::bool_<is_arguments_helper<T>::value>
{};

}} //namespace boost::clipp

#endif //BOOST_CLIPP_ARGUMENTS_HPP_HOLT_13082003
