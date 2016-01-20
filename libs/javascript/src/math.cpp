#include <boost/javascript/math.hpp>
#include <boost/clipp/class.hpp>
#include <ctime>
#include <limits>
#include <numeric>
#include "boost/clipp/container_wrapper.ipp"
#include <math.h>
#include <stdlib.h>
#include <complex>
#include <stdexcept>

using namespace boost::javascript;
using namespace boost::clipp;

void math::init(context* c)
{
    if(is_type_defined<math>(c)) return;
    class_<math,object> cls(c);
    cls.read("E",2.7182818284590452345);
    cls.read("LN10",2.302585092994046);
    cls.read("LN2",0.6931471805599453);
    cls.read("LOG2E",1.4426950408889634);
    cls.read("LOG10E",0.4342944819032518);
    cls.read("PI",3.1415926535897932);
    cls.read("SQRT1_2",0.7071067811865476);
    cls.read("SQRT2",1.4142135623730951);
    cls.read("i",std::complex<double>(0,1));

    cls.static_function("abs",abs);
    cls.static_function("acos",acos);
    cls.static_function("asin",asin);
    cls.static_function("atan",atan);
    cls.static_function("atan2",atan2);
    cls.static_function("ceil",ceil);
    cls.static_function("cos",cos);
    cls.static_function("exp",exp);
    cls.static_function("floor",floor);
    cls.static_function("log",log);
    cls.static_function("max",max);
    cls.static_function("min",min);
    cls.static_function("pow",pow);
    cls.static_function("random",random);
    cls.static_function("round",round);
    cls.static_function("sin",sin);
    cls.function("sqrt",&math::sqrt);
    cls.static_function("tan",tan);
    cls.static_function("add",add);

    cls.static_function("abs",(double(*)(const std::complex<double>&))std::abs);
    cls.static_function("real",(double(*)(const std::complex<double>&))std::real);
    cls.static_function("imag",(double(*)(const std::complex<double>&))std::imag);

    cls.static_function("sin",(std::complex<double>(*)(const std::complex<double>&))std::sin);
    cls.static_function("sinh",(std::complex<double>(*)(const std::complex<double>&))std::sinh);

    cls.static_function("cos",(std::complex<double>(*)(const std::complex<double>&))std::cos);
    cls.static_function("cosh",(std::complex<double>(*)(const std::complex<double>&))std::cosh);

    cls.static_function("sqrt",(std::complex<double>(*)(const std::complex<double>&))std::sqrt);
    cls.static_function("exp",(std::complex<double>(*)(const std::complex<double>&))std::exp);

    cls.static_function("log",(std::complex<double>(*)(const std::complex<double>&))std::log);
    cls.static_function("log10",(std::complex<double>(*)(const std::complex<double>&))std::log10);

    cls.static_function("pow",(std::complex<double>(*)(const std::complex<double>&,const double&))std::pow);
    cls.static_function("pow",(std::complex<double>(*)(const std::complex<double>&,const std::complex<double>&))std::pow);
    cls.static_function("pow",(std::complex<double>(*)(const double&,const std::complex<double>&))std::pow);

    cls.static_function("polar",(std::complex<double>(*)(const double&,const double&))std::polar).signature("x",arg("y")=0);
}

double math::add(const std::vector<double>& doubles)
{
    return std::accumulate(doubles.begin(),doubles.end(),0.0);
}

bool math::is_NaN(double value)
{
#ifdef BOOST_WIN32
    return _isnan(value)!=0;
#elif DJGPP
    return isnan(value)!=0;
#else
    throw std::runtime_error("is_NaN not supported!");
#endif
}

bool math::is_zero(double value)
{
    return value==0.0;
}

bool math::is_finite(double value)
{
#ifdef BOOST_WIN32
    return _finite(value)!=0;
#elif DJGPP
    return finite(value)!=0;
#else
    throw std::runtime_error("is_finite not supported!");
#endif
}

double math::NaN()
{
    return std::numeric_limits<double>::quiet_NaN();
}

double math::Infinity()
{
    return std::numeric_limits<double>::infinity();
}

double math::abs(double x)
{
    if(is_NaN(x)) return NaN();
    return fabs(x);
}

double math::acos(double x)
{
    if(is_NaN(x) || x>1 || x<-1) return NaN();
    return ::acos(x);
}

double math::asin(double x)
{
    if(is_NaN(x) || x>1 || x<1) return NaN();
    return ::asin(x);
}


double math::atan(double x)
{
    if(is_NaN(x)) return NaN();
    return ::atan(x);
}

double math::atan2(double y,double x)
{
    if(is_NaN(x) || is_NaN(y)) return NaN();
    return ::atan2(y,x);
}

double math::ceil(double x)
{
    return -floor(-x);
}

double math::cos(double x)
{
    if(is_NaN(x) || !is_finite(x)) return NaN();
    return ::cos(x);
}

double math::exp(double x)
{
    if(is_NaN(x)) return NaN();
    return ::exp(x);
}

double math::floor(double x)
{
    if(is_NaN(x)) return NaN();
    if(!is_finite(x)) return x;
    return ::floor(x);
}

double math::log(double x)
{
    if(is_NaN(x) || x<0) return NaN();
    return ::log(x);
}

double math::max(const std::valarray<double>& value)
{
    double max=-Infinity();
    for(size_t i=0;i<value.size();++i) {
        if(value[i]>max) max=value[i];
    }
    return max;
}

double math::min(const std::valarray<double>& value)
{
    double min=Infinity();
    for(size_t i=0;i<value.size();++i) {
        if(value[i]<min) min=value[i];
    }
    return min;
}

double math::pow(double x,double y)
{
    if(is_NaN(y)) return NaN();
    if(y==0) return 1;
    if(is_NaN(x)) return NaN();
    return ::pow(x,y);
}

double math::random()
{
    static bool seeded=false;
    if(!seeded) {
        srand( (unsigned)time(nullptr)*347889213 );
        seeded=true;
    }
    return double(rand())/double(RAND_MAX);
}

double math::round(double x)
{
    return long(x+0.5);
}

double math::sin(double x)
{
    if(is_NaN(x) || !is_finite(x)) return NaN();
    return ::sin(x);
}

valueP math::sqrt(double x)
{
    if(is_NaN(x)) return wrap(NaN(),get_context());
    if(x<0) return wrap(std::complex<double>(0,::sqrt(-x)),get_context());
    else return wrap(::sqrt(x),get_context());
}

double math::tan(double x)
{
    if(is_NaN(x) || !is_finite(x)) return NaN();
    return ::tan(x);
}
