// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_JAVASCRIPT_UNDEFINED_HPP_HOLT_15092003
#define BOOST_JAVASCRIPT_UNDEFINED_HPP_HOLT_15092003

#include "boost/javascript/config.hpp"
#include "boost/clipp/object.hpp"

namespace boost { namespace javascript {

class BOOST_JAVASCRIPT_EXPORT_IMPORT undefined : public clipp::object  
{
public:
    virtual ~undefined() {}
    undefined()  {}
    static void init(clipp::context* c);
    static bool is_undefined(clipp::valueP value);

    operator double() const;
    operator bool() const;
    operator std::string() const;
    bool operator ==(const clipp::value &) const {return false;}
};

}} // namespace boost::clipp

#endif //BOOST_JAVASCRIPT_UNDEFINED_HPP_HOLT_15092003

