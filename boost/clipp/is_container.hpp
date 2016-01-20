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
