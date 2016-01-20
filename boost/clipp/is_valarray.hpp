// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_IS_VALARRAY_HPP_HOLT_11082003
#define BOOST_CLIPP_IS_VALARRAY_HPP_HOLT_11082003

#include <valarray>
#include <boost/mpl/or.hpp>
#include <boost/type_traits/config.hpp>
#include <boost/type_traits/detail/yes_no_type.hpp>
#include <boost/call_traits.hpp>

namespace boost { namespace clipp {

template<typename T>struct is_valarray : mpl::false_ {};
template<typename T>struct is_valarray<std::valarray<T> > : mpl::true_ {};
template<typename T>struct is_valarray<std::valarray<T> const&> : mpl::true_ {};
template<typename T>struct is_valarray<std::valarray<T> &> : mpl::true_ {};
template<typename T>struct is_valarray<std::valarray<T> const*> : mpl::true_ {};
template<typename T>struct is_valarray<std::valarray<T> *> : mpl::true_ {};

}}//namespace boost::clipp

#endif //BOOST_CLIPP_IS_VALARRAY_HPP_HOLT_11082003
