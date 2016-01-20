// Copyright Peder Holt 2003-2004. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_IS_FUNCTION_POINTER_HPP_HOLT_28022004
#define BOOST_CLIPP_IS_FUNCTION_POINTER_HPP_HOLT_28022004

#include <boost/type_traits/is_function.hpp>

#include "boost/type_traits/detail/bool_trait_def.hpp"

namespace boost {
namespace detail {

#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
template<bool is_ref = true>
struct is_function_ptr_chooser
    : ::boost::type_traits::false_result
{
};

template <>
struct is_function_ptr_chooser<false>
{
    template< typename T > struct result_
        : ::boost::type_traits::is_function_ptr_helper<T>
    {
    };
};

template <typename T>
struct is_function_ptr_impl
    : is_function_ptr_chooser< ::boost::is_reference<T>::value >
        ::BOOST_NESTED_TEMPLATE result_<T>
{
};

#else

template <typename T>
struct is_function_ptr_impl
{
    static T* t;
    BOOST_STATIC_CONSTANT(
        bool, value = sizeof(::boost::type_traits::is_function_ptr_tester(*t))
        == sizeof(::boost::type_traits::yes_type)
        );
};

#endif

} // namespace detail

BOOST_TT_AUX_BOOL_TRAIT_DEF1(is_function_pointer,T,::boost::detail::is_function_ptr_impl<T>::value)

} // namespace boost

#include "boost/type_traits/detail/bool_trait_undef.hpp"

#endif //BOOST_CLIPP_IS_FUNCTION_POINTER_HPP_HOLT_28022004