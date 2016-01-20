#include <boost/javascript/undefined.hpp>
#include <boost/javascript/math.hpp>
#include <boost/clipp/class.hpp>
#include <boost/lexical_cast.hpp>

using namespace boost::javascript;
using namespace boost::clipp;

void undefined::init(context* c)
{
    if(is_type_defined<undefined>(c)) return;
    class_<undefined,object> cls("undefined",c);
    cls.converter(&undefined::operator double);
    cls.converter(&undefined::operator bool);
    cls.converter(&undefined::operator std::string);
    cls[self == value()];
};

undefined::operator double() const
{
    return math::NaN();
}

undefined::operator bool() const
{
    return false;
}

undefined::operator std::string() const
{
    return "undefined";
}

bool undefined::is_undefined(valueP value)
{
    if(value && typeid(*value)==typeid(undefined)) return true;
    return false;
}
