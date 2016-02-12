// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_JAVASCRIPT_NUMBER_HPP_HOLT_15092003
#define BOOST_JAVASCRIPT_NUMBER_HPP_HOLT_15092003

#include <boost/javascript/config.hpp>
#include <boost/clipp/object.hpp>

namespace boost { namespace javascript {

class BOOST_JAVASCRIPT_EXPORT_IMPORT number : public clipp::object  
{
public:
    number(const double& value) : value_(value) {}
    virtual ~number() {}
    static void init(clipp::context* c);
    double to_double() const {return value_;}
    int    to_int()    const {return int(value_);}
    unsigned int to_unsigned() const {return unsigned(value_);}

    operator double&() {return value_;}
    operator bool() const;

    std::string toString(int radix=10) const;
    double      toNumber() const {return value_;}
    std::string toLocaleString() const;

    const number& valueOf() const {return *this;}

    std::string toFixed(int fractionDigit=0) const;
    std::string toExponential(int fractionDigit=10) const;
    std::string toPrecision(int precision) const;

    friend BOOST_JAVASCRIPT_EXPORT_IMPORT clipp::valueP operator+(const number& lhs,clipp::valueP rhs);
    friend BOOST_JAVASCRIPT_EXPORT_IMPORT clipp::valueP operator+(clipp::valueP lhs,const number& rhs);
    friend inline double operator-(const number& lhs,double rhs) {return lhs.to_double() - rhs;}
    friend inline double operator-(double lhs,const number& rhs) {return lhs - rhs.to_double();}
    friend inline double operator*(const number& lhs,double rhs) {return lhs.to_double() * rhs;}
    friend inline double operator*(double lhs,const number& rhs) {return lhs * rhs.to_double();}
    friend inline double operator/(const number& lhs,double rhs) {return lhs.to_double() / rhs;}
    friend inline double operator/(double lhs,const number& rhs) {return lhs / rhs.to_double();}
    friend BOOST_JAVASCRIPT_EXPORT_IMPORT double operator%(const number& lhs,double rhs);
    friend BOOST_JAVASCRIPT_EXPORT_IMPORT double operator%(double lhs,const number& rhs);
    friend inline int operator>>(const number& lhs,unsigned int rhs) {return lhs.to_int() >> rhs;}
    friend inline int operator>>(int lhs,const number& rhs) {return lhs >> rhs.to_unsigned();}
    friend inline int operator<<(const number& lhs,unsigned int rhs) {return lhs.to_int() << rhs;}
    friend inline int operator<<(int lhs,const number& rhs) {return lhs << rhs.to_unsigned();}
    friend inline int operator&(const number& lhs,int rhs) {return lhs.to_int() & rhs;}
    friend inline int operator&(int lhs,const number& rhs) {return lhs & rhs.to_int();}
    friend inline int operator^(const number& lhs,int rhs) {return lhs.to_int() ^ rhs;}
    friend inline int operator^(int lhs,const number& rhs) {return lhs ^ rhs.to_int();}
    friend inline int operator|(const number& lhs,int rhs) {return lhs.to_int() | rhs;}
    friend inline int operator|(int lhs,const number& rhs) {return lhs | rhs.to_int();}

    friend inline bool operator<(const number& lhs,double rhs) {return lhs.to_double() < rhs;}
    friend inline bool operator>(const number& lhs,double rhs) {return lhs.to_double() > rhs;}
    friend inline bool operator==(const number& lhs,double rhs) {return lhs.to_double() == rhs;}

    friend BOOST_JAVASCRIPT_EXPORT_IMPORT bool operator<(const number& lhs,const std::string& rhs);
    friend BOOST_JAVASCRIPT_EXPORT_IMPORT bool operator>(const number& lhs,const std::string& rhs);
    friend BOOST_JAVASCRIPT_EXPORT_IMPORT bool operator==(const number& lhs,const std::string& rhs);

    double operator+() const {return value_;}
    double operator-() const {return -value_;}

    /*number & operator ++ () { ++value_; return *this; }
    double   operator ++ (int) { number old(*this); ++(*this); return old; }
    number & operator -- () { --value_; return *this; }
    double   operator -- (int) { number old(*this); --(*this); return old; }*/

    long operator~() const {return ~long(value_);}
    bool operator!() const {return !operator bool();}
private:
    double value_;
};

}} // namespace boost::clipp

#endif //BOOST_JAVASCRIPT_NUMBER_HPP_HOLT_15092003

