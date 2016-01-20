#include <boost/config.hpp>
#include <boost/detail/workaround.hpp>
#if BOOST_WORKAROUND( BOOST_MSVC, >= 1300)
 #pragma warning (disable:4244)
#endif

#include <boost/javascript/number.hpp>
#include <boost/javascript/math.hpp>
#include <boost/clipp/class.hpp>
#include <boost/lexical_cast.hpp>
#include <limits>
#include <sstream>
#include <math.h>
#include <stdexcept>
#include <boost/javascript/primitive.hpp>

using namespace boost::javascript;
using namespace boost::clipp;

void number::init(context* c)
{
    if(is_type_defined<number>(c)) return;
    class_<number,object,double> cls("Number",c);
    cls[constructor<const double&>()];
    
    cls.wrap(boost::type<char>());
    cls.wrap(boost::type<unsigned char>());
    cls.wrap(boost::type<signed char>());
    cls.wrap(boost::type<unsigned short>());
    cls.wrap(boost::type<short>());
    cls.wrap(boost::type<unsigned int>());
    cls.wrap(boost::type<int>());
    cls.wrap(boost::type<unsigned long>());
    cls.wrap(boost::type<long>());
    cls.wrap(boost::type<float>());

    cls.converter(&number::operator bool);

    cls.read("MAX_VALUE",std::numeric_limits<double>::max());
    cls.read("MIN_VALUE",std::numeric_limits<double>::min());
    cls.read("NaN",math::NaN());
    cls.read("NEGATIVE_INFINITY",-math::Infinity());
    cls.read("POSITIVE_INFINITY",math::Infinity());

    cls.function("toString",&number::toString).signature(arg("radix")=10);
    cls.function("toLocaleString",&number::toLocaleString);

    cls.function("valueOf",&number::valueOf);
    cls.function("toFixed",&number::toFixed).signature(arg("fractionDigit")=0);
    cls.function("toExponential",&number::toExponential).signature(arg("fractionDigit")=10);
    cls.function("toPrecision",&number::toPrecision).signature("precision");
    cls.function("toPrecision",&number::toLocaleString);

    cls[self + valueP()];
    cls[valueP() + self];
    cls[self - double()];
    cls[double() - self];
    cls[self * double()];
    cls[double() * self];
    cls[self / double()];
    cls[double() / self];
    cls[self % double()];
    cls[double() % self];
    cls[+self];
    cls[-self];
    cls[~self];
    cls[!self];
    cls[self < double()];
    cls[self > double()];
    cls[self == double()];
    cls[self < std::string()];
    cls[self > std::string()];
    cls[self == std::string()];
    cls[self >> unsigned()];
    cls[self << unsigned()];
    cls[int() >> self];
    cls[int() << self];
    cls[int() & self];
    cls[int() ^ self];
    cls[int() | self];
    cls[self & int()];
    cls[self ^ int()];
    cls[self | int()];
};

number::operator bool() const
{
    if(math::is_NaN(value_)) return false;
    if(math::is_zero(value_)) return false;
    return true;
}

double boost::javascript::operator%(const number& lhs,double rhs)
{
    double value=math::floor(lhs.to_double()/rhs);
    return lhs - double(value)*rhs;
}

double boost::javascript::operator%(double lhs,const number& rhs)
{
    double value=math::floor(lhs/rhs.to_double());
    return lhs - double(value)*rhs.to_double();
}

bool boost::javascript::operator<(const number& lhs,const std::string& rhs)
{
    double value=lexical_cast<double>(rhs);
    return lhs<value;
}

bool boost::javascript::operator>(const number& lhs,const std::string& rhs)
{
    double value=lexical_cast<double>(rhs);
    return lhs>value;
}

bool boost::javascript::operator==(const number& lhs,const std::string& rhs)
{
    double value=lexical_cast<double>(rhs);
    return lhs==value;
}

std::string number::toLocaleString() const
{
    return toString(10);
}

char get_digit(int digit)
{
    if(digit<=9) return digit+'0';
    else return digit-10+'A';
}

std::string number::toString(int radix) const
{
    if(math::is_NaN(value_)) return "NaN";
    if(math::is_zero(value_)) return "0";
    if(!math::is_finite(value_)) {
        if(value_<0.0) return "-Infinity";
        else return "Infinity";
    }
    if(radix<0) return "N/A";
    if(radix>36) return "N/A";
    if(radix==10) return boost::lexical_cast<std::string>(value_);
    if(value_>std::numeric_limits<long>::max() || value_<std::numeric_limits<long>::min()) return "N/A";
    switch(radix) {
    case 2: 
        {
            std::string val="0b";
            long bitfield=long(value_);
            unsigned long bit=1;
            bit <<= 31;
            bool in_number=false;
            while(bit) {
                if(bit&bitfield) {
                    in_number=true;
                    val+="1";
                }
                else if(in_number) {
                    val+="0";
                }
                bit>>=1;
            }
            return val;
        }
    case 8:
        {
            std::stringstream stream;
            stream << "0" << std::oct << long(value_) << std::ends;
            return stream.str();
        }
    case 16:
        {
            std::stringstream stream;
            stream << "0x" << std::hex << long(value_) << std::ends;
            return stream.str();
        }
    }
    std::string number;
    unsigned long generic_number=fabs(value_);
    if(generic_number==0) return boost::lexical_cast<std::string>(0);
    while(generic_number!=0) {
        number=get_digit(generic_number%radix)+number;
        generic_number/=radix;
    }
    if(value_<0) number="-"+number;
    return number;
}

std::string number::toFixed(int fractionDigit) const
{
    if(fractionDigit<0 || fractionDigit>20) throw std::range_error("Range Error");
    if(math::is_NaN(value_)) return "NaN";
    if(fabs(value_)>=1.e21) return toString();
    std::stringstream stream;
    stream.precision(fractionDigit);
    stream << std::fixed << value_ << std::ends;
    return stream.str();
}

std::string number::toExponential(int fractionDigit) const
{
    if(fractionDigit<0 || fractionDigit>20) throw std::range_error("Range Error");
    if(math::is_NaN(value_)) return "NaN";
    if(!math::is_finite(value_)) {
        if(value_<0) return "-Infinity";
        else return "Infinity";
    }
    std::stringstream stream;
    stream.precision(fractionDigit);
    stream << std::scientific << value_ << std::ends;
    return stream.str();
}

std::string number::toPrecision(int precision) const
{
    if(precision<1 || precision>20) throw std::range_error("Range Error");
    if(math::is_NaN(value_)) return "NaN";
    if(!math::is_finite(value_)) {
        if(value_<0) return "-Infinity";
        else return "Infinity";
    }
    std::stringstream stream;
    stream.precision(precision);
    stream.setf(std::ios_base::floatfield); 
    stream << value_ << std::ends;
    return stream.str();
}

valueP boost::javascript::operator+(const number& lhs,valueP rhs)
{
    valueP result=toPrimitive(rhs);
    if(isString(result)) {
        return wrap_struct<std::string>::wrap(lhs.toString()+unwrap<std::string>(result)(),lhs.get_context());
    }
    else {        
        unwrap<double> double_val(result);
        if(double_val.ok()) {
            return wrap_struct<double>::wrap(lhs.to_double()+double_val(),lhs.get_context());
        }
    }
    throw invalid_argument(bad_type_conversion(lhs.get_context()->type_name(rhs->type()),"Primitive"),2);
}

valueP boost::javascript::operator+(valueP lhs,const number& rhs)
{
    valueP result=toPrimitive(lhs);
    if(isString(result)) {
        return wrap_struct<std::string>::wrap(unwrap<std::string>(result)()+rhs.toString(),rhs.get_context());
    }
    else {
        unwrap<double> double_val(result);
        if(double_val.ok()) {
            return wrap_struct<double>::wrap(double_val()+rhs.to_double(),rhs.get_context());
        }
    }
    throw invalid_argument(bad_type_conversion(rhs.get_context()->type_name(lhs->type()),"Primitive"),1);
}