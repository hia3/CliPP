#include <boost/javascript/boolean.hpp>
#include <boost/clipp/class.hpp>
#include <boost/lexical_cast.hpp>

using namespace boost::javascript;
using namespace boost::clipp;

void boolean::init(context* c)
{
    if(is_type_defined<boolean>(c)) return;
    class_<boolean,object,bool> cls("Boolean",c);
    cls.constructor(arguments<bool>());

    cls.converter(&boolean::operator double);
    cls.converter(&boolean::operator char);
    cls.converter(&boolean::operator unsigned char);
    cls.converter(&boolean::operator signed char);
    cls.converter(&boolean::operator unsigned short);
    cls.converter(&boolean::operator short);
    cls.converter(&boolean::operator unsigned int);
    cls.converter(&boolean::operator int);
    cls.converter(&boolean::operator unsigned long);
    cls.converter(&boolean::operator long);
    cls.converter(&boolean::operator float);
    cls.converter(&boolean::operator std::string);

    cls[self == self];
    cls[!self];

    cls.function("toString",&boolean::operator std::string);
};

boolean::operator std::string() const
{
    if(value_) return "true";
    else return "false";
}
