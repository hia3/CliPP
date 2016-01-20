// Copyright Peder Holt 2003-2004. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_JAVASCRIPT_JS_STRING_HPP_HOLT_25092003
#define BOOST_JAVASCRIPT_JS_STRING_HPP_HOLT_25092003

#include "boost/javascript/config.hpp"
#include "boost/clipp/object.hpp"
#include <string>
#include <valarray>
#include <limits>

namespace boost { namespace javascript {

class BOOST_JAVASCRIPT_EXPORT_IMPORT js_string : public clipp::object  
{
public:
    js_string(const std::string& value) : value_(value) {}
    js_string(const char* value) : value_(value) {}
    js_string(const js_string& value) : clipp::object(), value_(value.value_) {}
    js_string() {}

//Type conversion
    operator const char*() const {return value_.c_str();}
    operator std::string&() {return value_;}

//Properties
    const js_string& toString() const {return *this;}
    double           toNumber() const;

//Member operations
    std::string charAt(int pos) const;
    double charCodeAt(int pos) const;
    std::string concat(const std::valarray<std::string>& stringN) const;
    int indexOf(const std::string& searchString,int position=0) const;
    int lastIndexOf(const std::string& searchString,int position=std::numeric_limits<int>::max()) const;

//Static operations
    static std::string fromCharCode(const std::valarray<char>& charI);

//Operators
    friend inline std::string operator+(js_string const& lhs,std::string const & rhs) {return std::string(lhs) + rhs;}
    friend inline std::string operator+(std::string const& lhs,js_string const & rhs) {return lhs + std::string(rhs);}
    friend BOOST_JAVASCRIPT_EXPORT_IMPORT std::string operator+(js_string const& lhs,double const & rhs);
    friend BOOST_JAVASCRIPT_EXPORT_IMPORT std::string operator+(double const& lhs,js_string const & rhs);
    friend inline bool operator==(js_string const& lhs,std::string const & rhs) {return std::string(lhs) == rhs;}
    friend inline bool operator<(js_string const& lhs,std::string const & rhs) {return std::string(lhs) < rhs;}
    friend inline bool operator>(js_string const& lhs,std::string const & rhs) {return std::string(lhs) > rhs;}

    virtual ~js_string() {}
    static void init(clipp::context* c);
private:
    std::string value_;
};

}} // namespace boost::clipp

#endif //BOOST_JAVASCRIPT_JS_STRING_HPP_HOLT_25092003

