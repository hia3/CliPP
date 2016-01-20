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

template<typename Constructor>
struct constructor_generator {
    explicit constructor_generator(Constructor constructor) : constructor_(std::move(constructor)) {}
    typedef valueP result_type;

    //Constructor
    template<typename R, typename cv_tagT, typename... Args>
    result_type execute
    (
        context* c,
        boost::type<R>,
        constructor_tag const&,
        cv_tagT,
        Args && ... args
    ) const
    {
        return wrap_struct<R*>::wrap(new R(std::forward<Args>(args)()...), c);
    }

    //Constructor no new
    template<typename R, typename cv_tagT, typename... Args>
    result_type execute
    (
        context* c,
        boost::type<R>,
        constructor_no_new_tag const&,
        cv_tagT,
        Args && ... args
    ) const
    {
        return wrap_struct<const R&>::wrap(R(std::forward<Args>(args)()...), c);
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
        return wrap_struct<R>::wrap(constructor_(std::forward<Args>(args)()...), c);
    }

    Constructor constructor_;
};

}}} //namespace boost::clipp::detail
