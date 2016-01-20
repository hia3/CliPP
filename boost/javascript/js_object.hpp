// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_JAVASCRIPT_OBJECT_HPP_HOLT_23112003
#define BOOST_JAVASCRIPT_OBJECT_HPP_HOLT_23112003

#include <boost/javascript/config.hpp>
#include <boost/javascript/primitive.hpp>
#include <boost/clipp/scope.hpp>

namespace boost { namespace javascript {

class BOOST_JAVASCRIPT_EXPORT_IMPORT js_object : public clipp::scope  
{
public:
    js_object()  {}
    virtual ~js_object() {}
    static void init(clipp::context* c);
    clipp::valueP defaultValue(PreferredType::Hint hint=PreferredType::Hint::No);
    virtual clipp::detail::converterP get_converter_from_this(const clipp::type_detail& to,clipp::precedence p,clipp::valueP& wrapped);

    friend BOOST_JAVASCRIPT_EXPORT_IMPORT clipp::valueP operator+(const js_object& lhs,clipp::valueP rhs);
    friend BOOST_JAVASCRIPT_EXPORT_IMPORT clipp::valueP operator+(clipp::valueP lhs,const js_object& rhs);
    friend inline double operator-(const js_object& lhs,double rhs) {return toNumber(lhs)-rhs;}
    friend inline double operator-(double lhs,const js_object& rhs) {return lhs-toNumber(rhs);}
    friend inline double operator*(const js_object& lhs,double rhs) {return toNumber(lhs)*rhs;}
    friend inline double operator*(double lhs,const js_object& rhs) {return lhs*toNumber(rhs);}
    friend inline double operator/(const js_object& lhs,double rhs) {return toNumber(lhs)/rhs;}
    friend inline double operator/(double lhs,const js_object& rhs) {return lhs/toNumber(rhs);}
    friend BOOST_JAVASCRIPT_EXPORT_IMPORT double operator%(const js_object& lhs,double rhs);
    friend BOOST_JAVASCRIPT_EXPORT_IMPORT double operator%(double lhs,const js_object& rhs);
    friend inline int operator>>(const js_object& lhs,unsigned int rhs) {return int(toNumber(lhs))>>rhs;}
    friend inline int operator>>(int lhs,const js_object& rhs) {return lhs>>int(toNumber(rhs));}
    friend inline int operator<<(const js_object& lhs,unsigned int rhs) {return int(toNumber(lhs))<<rhs;}
    friend inline int operator<<(int lhs,const js_object& rhs) {return lhs<<int(toNumber(rhs));}
    friend inline int operator&(const js_object& lhs,int rhs) {return int(toNumber(lhs))&rhs;}
    friend inline int operator&(int lhs,const js_object& rhs) {return lhs&int(toNumber(rhs));}
    friend inline int operator^(const js_object& lhs,int rhs) {return int(toNumber(lhs))^rhs;}
    friend inline int operator^(int lhs,const js_object& rhs) {return lhs^int(toNumber(rhs));}
    friend inline int operator|(const js_object& lhs,int rhs) {return int(toNumber(lhs))|rhs;}
    friend inline int operator|(int lhs,const js_object& rhs) {return lhs|int(toNumber(rhs));}

    double operator+() const {return toNumber(*this);}
    double operator-() const {return -toNumber(*this);}
    int operator~() const {return ~int(toNumber(*this));}
};

}} // namespace boost::clipp

#endif //BOOST_JAVASCRIPT_OBJECT_HPP_HOLT_23112003

