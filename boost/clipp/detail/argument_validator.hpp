// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#pragma once

namespace boost { namespace clipp { namespace detail {

struct argument_validator {
    typedef bool result_type;

    template<typename R, typename CT, typename CV, typename... As>
    result_type execute
        (
            context* c,
            R r,
            CT const& ct,
            CV cv,
            As && ... as
        ) const
    {
        std::initializer_list<bool> results = { std::forward<As>(as).ok()... };
        return std::all_of(std::begin(results), std::end(results), std::identity<bool>());
    }
};

}}} //namespace boost::clipp::detail
