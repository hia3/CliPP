// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_CLASS_WRAPPER_HPP_HOLT_31082003
# define BOOST_CLIPP_CLASS_WRAPPER_HPP_HOLT_31082003

#include <boost/clipp/object.hpp>
#include <boost/clipp/policy_tag.hpp>

namespace boost { namespace clipp { 

template<typename T>
class class_wrapper : public object {
public:
    class_wrapper(const T& rhs) : object_(rhs) {}
    virtual ~class_wrapper() {}
    operator T&() {return object_;}
    static void init(context* c) {}
private:
    T object_;
};

struct direct_storage_policy : storage_policy_tag
{
    template<typename T>
    struct apply {
        typedef class_wrapper<T> type;
    };
    typedef detail::constructor_no_new_tag create_tag;
};

}} //namespace boost::clipp::detail

#endif //BOOST_CLIPP_CLASS_WRAPPER_HPP_HOLT_31082003

