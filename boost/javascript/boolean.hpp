// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_JAVASCRIPT_BOOLEAN_HPP_HOLT_15092003
#define BOOST_JAVASCRIPT_BOOLEAN_HPP_HOLT_15092003

#include <boost/javascript/config.hpp>
#include <boost/clipp/object.hpp>

namespace boost { namespace javascript {

class BOOST_JAVASCRIPT_EXPORT_IMPORT boolean : public clipp::object  
{
public:
    boolean(bool value) : value_(value) {}
    virtual ~boolean() {}
    static void init(clipp::context* c);

    operator bool&() {return value_;}
    operator double() {return value_;}
    operator char() {return value_;}
    operator unsigned char() {return value_;}
    operator signed char() {return value_;}
    operator unsigned short() {return value_;}
    operator short() {return value_;}
    operator unsigned int() {return value_;}
    operator int() {return value_;}
    operator unsigned long() {return value_;}
    operator long() {return value_;}
    operator float() {return value_;}
    operator std::string() const;
    bool operator ==(const boolean &b) const {return value_ == b.value_;}
    bool operator!() const {return !value_;}
private:
    bool value_;
};

}} // namespace boost::clipp

#endif //BOOST_JAVASCRIPT_NUMBER_HPP_HOLT_15092003

