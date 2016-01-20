// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_IS_CONTAINER_HPP_HOLT_29072003
#define BOOST_CLIPP_IS_CONTAINER_HPP_HOLT_29072003


#include <list>
#include <valarray>
#include <set>
#include <vector>
#include "boost/mpl/or.hpp"
#include "boost/type_traits/config.hpp"
#include "boost/type_traits/detail/yes_no_type.hpp"
#include "boost/clipp/is_valarray.hpp"

namespace boost { 
namespace clipp {

#if BOOST_WORKAROUND( BOOST_MSVC, <= 1300)
namespace detail {
template<typename T>
struct is_set_helper {
    static type_traits::no_type BOOST_TT_DECL _m_check(...);
    template<typename K,typename Pr,typename A>
    static type_traits::yes_type BOOST_TT_DECL _m_check(void (*)(std::set<K,Pr,A> const &));

    typedef void (*fn)(T const&);                                                          

    BOOST_STATIC_CONSTANT(bool, value = 
        sizeof(_m_check(((fn)0)) ) == sizeof(type_traits::yes_type)
    );
};
}//namespace detail


template<typename T>
struct is_set : mpl::bool_<detail::is_set_helper<T>::value>
{};

namespace detail {
template <typename T>
struct is_vector_helper
{
    static type_traits::no_type BOOST_TT_DECL _m_check(...);
    template<typename Ty,typename A>
    static type_traits::yes_type BOOST_TT_DECL _m_check(void (*)(std::vector<Ty,A> const&));

    typedef void (*fn)(T const&);                                                          

    BOOST_STATIC_CONSTANT(bool, value = 
        sizeof(_m_check(((fn)0)) ) == sizeof(type_traits::yes_type)
    );
};
}//namespace detail

template<typename T>
struct is_vector : mpl::bool_<detail::is_vector_helper<T>::value>
{};

namespace detail {
template <typename T>
struct is_list_helper
{
    static type_traits::no_type BOOST_TT_DECL _m_check(...);
    template<typename Ty,typename A>
    static type_traits::yes_type BOOST_TT_DECL _m_check(void (*)(std::list<Ty,A> const&));
    typedef void (*fn)(T const&);                                                          

    BOOST_STATIC_CONSTANT(bool, value = 
        sizeof(_m_check(((fn)0)) ) == sizeof(type_traits::yes_type)
    );
};
}//namespace detail

template<typename T>
struct is_list : mpl::bool_<detail::is_list_helper<T>::value>
{};
#else
template<typename T>struct is_set : mpl::false_ {};
template<typename K,typename Pr,typename A>struct is_set<std::set<K,Pr,A> > : mpl::true_ {};
template<typename K,typename Pr,typename A>struct is_set<std::set<K,Pr,A> const&> : mpl::true_ {};
template<typename K,typename Pr,typename A>struct is_set<std::set<K,Pr,A> &> : mpl::true_ {};

template<typename T>struct is_vector : mpl::false_ {};
template<typename Ty,typename A>struct is_vector<std::vector<Ty,A> > : mpl::true_ {};
template<typename Ty,typename A>struct is_vector<std::vector<Ty,A> const&> : mpl::true_ {};
template<typename Ty,typename A>struct is_vector<std::vector<Ty,A> &> : mpl::true_ {};

template<typename T>struct is_list : mpl::false_ {};
template<typename Ty,typename A>struct is_list<std::list<Ty,A> > : mpl::true_ {};
template<typename Ty,typename A>struct is_list<std::list<Ty,A> const&> : mpl::true_ {};
template<typename Ty,typename A>struct is_list<std::list<Ty,A> &> : mpl::true_ {};

#endif

template <typename T>
struct is_container : public 
    mpl::or_<
        is_vector<T>,
        is_set<T>,
        is_list<T> 
    >
{};
}}//namespace boost::clipp

#endif //BOOST_CLIPP_IS_CONTAINER_HPP_HOLT_29072003
