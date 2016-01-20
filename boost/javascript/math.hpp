// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_JAVASCRIPT_MATH_HPP_HOLT_15092003
#define BOOST_JAVASCRIPT_MATH_HPP_HOLT_15092003

#include <boost/javascript/config.hpp>
#include <boost/clipp/object.hpp>
#include <valarray>
#include <vector>

namespace boost { namespace javascript {

class BOOST_JAVASCRIPT_EXPORT_IMPORT math : public clipp::object  
{
public:
    math()  {}
    virtual ~math() {}
    
    static void init(clipp::context* c);

    static bool is_NaN(double value);
    static bool is_zero(double value);
    static bool is_finite(double value);
    static double NaN();
    static double Infinity();

    static double add(const std::vector<double>& doubles);
    static double abs(double x);
    static double acos(double x);
    static double asin(double x);
    static double atan(double x);
    static double atan2(double y,double x);
    static double ceil(double x);
    static double cos(double x);
    static double exp(double x);
    static double floor(double x);
    static double log(double x);
    static double max(const std::valarray<double>& value);
    static double min(const std::valarray<double>& value);
    static double pow(double x,double y);
    static double random();
    static double round(double x);
    static double sin(double x);
    clipp::valueP sqrt(double x);
    static double tan(double x);
};

}} // namespace boost::clipp

#endif //BOOST_JAVASCRIPT_NUMBER_HPP_HOLT_15092003

