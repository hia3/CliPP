// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_BACK_REFERENCE_HPP_HOLT_21092003
#define BOOST_CLIPP_BACK_REFERENCE_HPP_HOLT_21092003

#include <boost/clipp/value.hpp>
#include <boost/call_traits.hpp>

namespace boost {namespace clipp {

template<typename T>
struct back_reference {
    typedef T reference_type;
    back_reference(const valueP& v,reference_type r) : value_(v),reference_(r) {}
    reference_type get() {return reference_;}
    context* get_context() {return value_->get_context();}
    valueP   get_value() {return value_;}
private:
    valueP value_;
    reference_type  reference_;
};

BOOST_CLIPP_IS_WRAPPED_TEMPLATE(clipp_ptr);
BOOST_CLIPP_IS_WRAPPED_TEMPLATE(back_reference);

namespace detail {

template<typename T>
struct default_type_selector {
    typedef T type;
};

template<typename T>
struct back_reference_selector {
    typedef typename T::reference_type type;
};

}

template<typename T>
struct result_type_selector
{
    typedef typename mpl::if_<
        is_back_reference<T>,
        typename detail::back_reference_selector<T>,
        typename detail::default_type_selector<T>
    >::type func_;
    typedef typename func_::type type;
};

}} //namespace boost::clipp

#endif //BOOST_CLIPP_BACK_REFERENCE_HPP_HOLT_21092003

