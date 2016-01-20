#include <boost/javascript/primitive.hpp>
#include <boost/javascript/math.hpp>
#include <boost/javascript/js_object.hpp>
#include <boost/javascript/boolean.hpp>
#include <boost/javascript/number.hpp>
#include <boost/javascript/js_string.hpp>
#include <boost/javascript/date.hpp>
#include <boost/javascript/array.hpp>
#include <boost/javascript/reference.hpp>
#include <boost/clipp/context.hpp>
#include <boost/clipp/enum.hpp>

using namespace boost::clipp;
using namespace boost::javascript;

void PreferredType::init(context* c)
{
    enum_<Hint> e("PreferredType",c);
    e.value("String",PreferredType::Hint::String);
    e.value("Number",PreferredType::Hint::Number);
    e.value("NoHint",PreferredType::Hint::No);
}

valueP boost::javascript::toPrimitive(valueP input,PreferredType::Hint hint)
{
    valueP function=input["defaultValue"];
    if(function) {
        valueP result=function(hint);
        if(result) return result;
    }
    return input;
}

double boost::javascript::toNumber(clipp::valueP input)
{
    try {
        if(isString(input)) return unwrap<double>(input["toNumber"]())();
        if(isObject(input)) {
            valueP result=toPrimitive(input,PreferredType::Hint::Number);
            if(isObject(result)) return math::NaN();
            else return toNumber(result);
        }
        else return unwrap<double>(input)();
    }
    catch(...) {
        throw bad_type_conversion(input->get_context()->type_name(input->type()),"Number");
    }
}

std::string boost::javascript::toString(clipp::valueP input)
{
    try {
        if(isNumber(input)) return unwrap<std::string>(input["toString"]())();
        if(isObject(input)) {
            valueP result=toPrimitive(input,PreferredType::Hint::String);
            if(isObject(result)) return "Object";
            else return toString(result);
        }
        else return unwrap<std::string>(input)();
    }
    catch(...) {
        throw bad_type_conversion(input->get_context()->type_name(input->type()),"String");
    }
}

bool boost::javascript::isObject(clipp::valueP input)
{
    return unwrap<js_object>(input).ok() || unwrap<array>(input).ok() || unwrap<date>(input).ok();
}

bool boost::javascript::isString(clipp::valueP input)
{
    return unwrap<js_string>(input).ok();
}

bool boost::javascript::isNumber(clipp::valueP input)
{
    return unwrap<number>(input).ok();
}

bool boost::javascript::isBoolean(clipp::valueP input)
{
    return unwrap<boolean>(input).ok();
}
