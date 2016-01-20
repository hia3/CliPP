// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_LIST_HPP_HOLT_14082003
#define BOOST_CLIPP_LIST_HPP_HOLT_14082003

#include <boost/clipp/config.hpp>


#if BOOST_CLIPP_MAX_ARITY>47
#   error arities greater than 47 are not allowed due to limitations in boost::mpl
#elif BOOST_CLIPP_MAX_ARITY>37
#   define BOOST_MPL_LIMIT_LIST_SIZE 50
#   define BOOST_MPL_NO_PREPROCESSED_HEADERS
#elif BOOST_CLIPP_MAX_ARITY>27
#   define BOOST_MPL_LIMIT_LIST_SIZE 40
#   define BOOST_MPL_NO_PREPROCESSED_HEADERS
#elif BOOST_CLIPP_MAX_ARITY>17
#   define BOOST_MPL_LIMIT_LIST_SIZE 30
#   define BOOST_MPL_NO_PREPROCESSED_HEADERS
#elif BOOST_CLIPP_MAX_ARITY>7
#   define BOOST_MPL_LIMIT_LIST_SIZE 20
#   define BOOST_MPL_NO_PREPROCESSED_HEADERS
#else
#   define BOOST_MPL_LIMIT_LIST_SIZE 10
#endif

#include <boost/mpl/list.hpp>


#endif //BOOST_CLIPP_LIST_HPP_HOLT_14082003

