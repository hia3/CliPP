// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_IS_VALARRAY_HPP_HOLT_11082003
#define BOOST_CLIPP_IS_VALARRAY_HPP_HOLT_11082003

#include <valarray>
#include "boost/mpl/or.hpp"
#include "boost/type_traits/config.hpp"
#include "boost/type_traits/detail/yes_no_type.hpp"
#include <boost/call_traits.hpp>

namespace boost { namespace clipp {

#if BOOST_WORKAROUND( BOOST_MSVC, <= 1300)
namespace detail {
template<typename T>
struct is_valarray_helper {
    static type_traits::no_type BOOST_TT_DECL _m_check(...);
    template<class U>
    static type_traits::yes_type BOOST_TT_DECL _m_check(std::valarray<U> const& (*)());    
    template<class U>
    static type_traits::yes_type BOOST_TT_DECL _m_check(std::valarray<U> const* (*)());    
    template<class U>
    static type_traits::yes_type BOOST_TT_DECL _m_check(std::valarray<U> & (*)());    
    template<class U>
    static type_traits::yes_type BOOST_TT_DECL _m_check(std::valarray<U> * (*)());    

    typedef typename call_traits<T>::const_reference (*fn)();                                                          

    BOOST_STATIC_CONSTANT(bool, value = 
        sizeof(_m_check(((fn)0)) ) == sizeof(type_traits::yes_type)
    );
};
}//namespace detail


template<typename T>
struct is_valarray : public
    mpl::bool_<detail::is_valarray_helper<T>::value>
{};
#else
template<typename T>struct is_valarray : mpl::false_ {};
template<typename T>struct is_valarray<std::valarray<T> > : mpl::true_ {};
template<typename T>struct is_valarray<std::valarray<T> const&> : mpl::true_ {};
template<typename T>struct is_valarray<std::valarray<T> &> : mpl::true_ {};
template<typename T>struct is_valarray<std::valarray<T> const*> : mpl::true_ {};
template<typename T>struct is_valarray<std::valarray<T> *> : mpl::true_ {};
#endif

}}//namespace boost::clipp

#endif //BOOST_CLIPP_IS_VALARRAY_HPP_HOLT_11082003
