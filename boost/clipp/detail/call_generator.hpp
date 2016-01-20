// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#pragma once

#include <boost/clipp/detail/signature.hpp>
#include <boost/clipp/wrap.hpp>

#include <boost/type.hpp>

namespace boost { namespace clipp { namespace detail {

template<typename Fn>
struct call_generator {
    call_generator(Fn fn) : fn_(fn) {}
    typedef valueP result_type;

    //Member function
    template<typename R, typename cv_tagT, typename Object, typename... Args>
    result_type execute
    (
        context* c,
        boost::type<R>,
        member_function_tag const& ct,
        cv_tagT,
        Object && object,
        Args && ... args
    ) const
    {
        return wrap_struct<R>::wrap((std::forward<Object>(object)().*fn_)(std::forward<Args>(args)()...), c);
    }
    //Void member function
    template<typename cv_tagT, typename Object, typename... Args>
    result_type execute
    (
        context* c,
        mpl::void_,
        member_function_tag const& ct,
        cv_tagT,
        Object && object,
        Args && ... args
    ) const
    {
        (std::forward<Object>(object)().*fn_)(std::forward<Args>(args)()...);
        return nullptr;
    }

    //Free function
    template<typename R, typename cv_tagT, typename... Args>
    result_type execute
    (
        context* c,
        boost::type<R>,
        free_function_tag const&,
        cv_tagT,
        Args && ... args
    ) const
    {
        return wrap_struct<R>::wrap(fn_(std::forward<Args>(args)()...), c);
    }
    //Void free function
    template<typename cv_tagT, typename... Args>
    result_type execute
    (
        context* c,
        mpl::void_,
        free_function_tag const&,
        cv_tagT,
        Args && ... args
    ) const
    {
        fn_(std::forward<Args>(args)()...);
        return nullptr;
    }

    Fn fn_;
};

}}} //namespace boost::clipp::detail
