// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_INDIRECTION_CATEGORY_HPP_HOLT_31082003
# define BOOST_CLIPP_INDIRECTION_CATEGORY_HPP_HOLT_31082003

# include <boost/type_traits/is_pointer.hpp>
# include <boost/type_traits/is_reference.hpp>

namespace boost { namespace clipp { namespace detail { 

template <bool is_pointer_, bool is_reference_>
struct indirection_tag
{
    BOOST_STATIC_CONSTANT(bool, is_pointer = is_pointer_);
    BOOST_STATIC_CONSTANT(bool, is_reference = is_reference_);
};

typedef indirection_tag<false,false> direct_;
typedef indirection_tag<true,false> pointer_;
typedef indirection_tag<false,true> reference_;
typedef indirection_tag<true,true> reference_to_pointer;

template <class T>
struct indirection_category
{
    BOOST_STATIC_CONSTANT(bool, p = is_pointer<T>::value);
    BOOST_STATIC_CONSTANT(bool, r = is_reference<T>::value);
    typedef indirection_tag<p,r> type;
};

}}} // namespace boost::clipp::detail

#endif // BOOST_CLIPP_INDIRECTION_CATEGORY_HPP_HOLT_31082003

