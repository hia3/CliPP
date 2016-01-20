// Copyright Peder Holt 2005. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_JAVASCRIPT_PRIMITIVE_HPP_20082005
#define BOOST_JAVASCRIPT_PRIMITIVE_HPP_20082005

#include <boost/javascript/config.hpp>
#include <boost/clipp/clipp_ptr.hpp>

namespace boost { namespace clipp {
    class context;
    class value;
    typedef clipp_ptr<value> valueP;    
}}

namespace boost { namespace javascript {

    struct BOOST_JAVASCRIPT_EXPORT_IMPORT PreferredType {
        enum Hint {
            NoHint,
            String,
            Number,
        };
        static void init(clipp::context* c);
    };

    clipp::valueP BOOST_JAVASCRIPT_EXPORT_IMPORT toPrimitive(clipp::valueP input,PreferredType::Hint hint=PreferredType::NoHint);
    
    template<typename T>
    clipp::valueP toPrimitive(const T& input,PreferredType::Hint hint=PreferredType::NoHint)
    {
        return toPrimitive(wrap_struct<const T&>::wrap(input,input.get_context()),hint);
    }

    double BOOST_JAVASCRIPT_EXPORT_IMPORT toNumber(clipp::valueP input);
    template<typename T>
    double toNumber(const T& input)
    {
        return toNumber(wrap_struct<const T&>::wrap(input,input.get_context()));
    }

    std::string BOOST_JAVASCRIPT_EXPORT_IMPORT toString(clipp::valueP input);
    template<typename T>
    std::string toString(const T& input)
    {
        return toString(wrap_struct<const T&>::wrap(input,input.get_context()));
    }

    bool BOOST_JAVASCRIPT_EXPORT_IMPORT isObject(clipp::valueP input);
    bool BOOST_JAVASCRIPT_EXPORT_IMPORT isString(clipp::valueP input);
    bool BOOST_JAVASCRIPT_EXPORT_IMPORT isNumber(clipp::valueP input);
    bool BOOST_JAVASCRIPT_EXPORT_IMPORT isBoolean(clipp::valueP input);
}}

#endif

