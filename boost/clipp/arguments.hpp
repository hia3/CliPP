// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_ARGUMENTS_HPP_HOLT_13082003
#define BOOST_CLIPP_ARGUMENTS_HPP_HOLT_13082003

#include <boost/clipp/config.hpp>

#include <boost/mpl/vector.hpp>

namespace boost { namespace clipp {

template<typename... A>
struct arguments : mpl::vector<A...> {};

template<typename... A>
struct constructor : arguments<A...> {};

}} //namespace boost::clipp

#endif //BOOST_CLIPP_ARGUMENTS_HPP_HOLT_13082003
