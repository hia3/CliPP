#include <boost/javascript/js_string.hpp>
#include <boost/javascript/math.hpp>
#include <boost/clipp/class.hpp>

using namespace boost::javascript;
using namespace boost::clipp;

void js_string::init(context* c)
{
    class_<js_string,object,std::string> cls("String",c);
//Type conversion
    cls.wrap(boost::type<char>());
    cls.converter(arguments<const std::string&>());
    cls.converter(arguments<const js_string&>());
    cls.converter(&js_string::operator std::string&);

//Construction
    cls[constructor<std::string const&>()];
    cls[constructor<>()];

//Operators
    cls[self + std::string()];
    cls[std::string() + self];
    cls[self + double()];
    cls[double() + self];
    cls[self == std::string()];
    cls[self > std::string()];
    cls[self < std::string()];

//Member operations
    cls.function("charAt",&js_string::charAt);
    cls.function("charCodeAt",&js_string::charCodeAt);
    cls.function("concat",&js_string::concat);
    cls.function("indexOf",&js_string::indexOf).signature("searchString",arg("position")=0);
    cls.function("lastIndexOf",&js_string::lastIndexOf).signature("searchString",arg("position")=std::numeric_limits<int>::max());

//Properties
    cls.read("toString",&js_string::toString);
    cls.read("toNumber",&js_string::toNumber);
    cls.read("valueOf",&js_string::toString);

//Static operations
    cls.static_function("fromCharCode",fromCharCode);
};

double js_string::toNumber() const
{
    return lexical_cast<double>(value_);
}

std::string boost::javascript::operator+(js_string const& lhs,double const & rhs)
{
    return lhs+lexical_cast<std::string>(rhs);
}

std::string boost::javascript::operator+(double const& lhs,js_string const & rhs)
{
    return lexical_cast<std::string>(lhs)+rhs;
}

std::string js_string::fromCharCode(const std::valarray<char>& charI)
{
    std::string text;
    for(size_t i=0;i<charI.size();++i) {
        text+=charI[i];
    }
    return text;
}

std::string js_string::charAt(int pos) const
{
    if(pos<0 || pos>=int(value_.size())) return "";
    return std::string(1,value_.at(pos));
}

double js_string::charCodeAt(int pos) const
{
    if(pos<0 || pos>=int(value_.size())) return math::NaN();
    return value_[pos];
}

std::string js_string::concat(const std::valarray<std::string>& stringN) const
{
    std::string result=value_;
    for(size_t i=0;i<stringN.size();++i) {
        result+=stringN[i];
    }
    return result;
}

int js_string::indexOf(const std::string& searchString,int position) const
{
    if(position<0) position=0;
    return value_.find(searchString,position);
}

int js_string::lastIndexOf(const std::string& searchString,int position) const
{
    if(position<0) position=0;
    if(position>int(searchString.size())) position=searchString.size();
    return value_.rfind(searchString,position);
}

