// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_DETAIL_MAKE_VALUE_TYPE_HPP_HOLT_31082003
# define BOOST_CLIPP_DETAIL_MAKE_VALUE_TYPE_HPP_HOLT_31082003

#include <boost/mpl/if.hpp>
#include <boost/clipp/arithmetic_wrapper.hpp>
#include <boost/clipp/class_wrapper.hpp>
#include <boost/clipp/enum_wrapper.hpp>
#include <boost/type_traits/is_base_and_derived.hpp>

namespace boost { namespace clipp { namespace detail {

template<typename T>
struct unknown_wrapper
{
};

//T is an undecorated type
template<typename T>
struct make_value_type {
    typedef typename 
        mpl::if_<
            is_class<T>,
            mpl::if_<
                is_base_and_derived<value,T>,
                T,
                class_wrapper<T>
                >::type,
            mpl::if_<
                is_arithmetic<T>,
                arithmetic_wrapper<T>,
                mpl::if_<
                    is_enum<T>,
                    enum_wrapper<T>,
                    unknown_wrapper<T>
                >::type
            >::type
        >::type type;
};


}}} //namespace boost::clipp::detail

#endif //BOOST_CLIPP_DETAIL_MAKE_VALUE_TYPE_HPP_HOLT_31082003