// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#pragma once

#include <boost/clipp/value.hpp>
#include <boost/clipp/detail/register_type.hpp>

namespace boost { namespace clipp { namespace detail {

struct register_types {
    register_types() {}
    typedef bool result_type;

    //Return traits
    template<typename R, typename fn_tagT, typename cv_tagT, typename... Args>
    result_type execute
    (
        context* c,
        boost::type<R>,
        fn_tagT,
        cv_tagT,
        Args && ...
    ) const
    {
        register_all(c, BOOST_CLIPP_UNWRAP_TYPE(R), BOOST_CLIPP_UNWRAP_TYPE(typename Args::result_type)...);
        return true;
    }
    //Void traits
    template<typename fn_tagT, typename cv_tagT, typename... Args>
    result_type execute
    (
        context* c,
        mpl::void_,
        fn_tagT,
        cv_tagT,
        Args && ...
    ) const
    {
        register_all(c, BOOST_CLIPP_UNWRAP_TYPE(typename Args::result_type)...);
        return true;
    }
private:
    template<typename... Ts>
    static void register_all(context* c, Ts && ... ts)
    {
        std::initializer_list<bool> r{ register_traits(ts, c)... };
        (void)r;
    }
};

}}} //namespace boost::clipp::detail
