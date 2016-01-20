#include <boost/javascript/js_object.hpp>
#include <boost/clipp/class.hpp>
#include <boost/clipp/detail/operator_invoker.hpp>
#include <boost/javascript/math.hpp>

using namespace boost::clipp;
using namespace boost::javascript;

void js_object::init(context* c)
{
    class_<js_object,scope> cls("Object",c);
    cls[constructor<>()];
    cls.function("defaultValue",&js_object::defaultValue).signature(arg("hint")=PreferredType::NoHint);
    cls[self+valueP()];
    cls[valueP()+self];
    cls[self-double()];
    cls[double()-self];
    cls[self * double()];
    cls[double() * self];
    cls[self / double()];
    cls[double() / self];
    cls[self % double()];
    cls[double() % self];
    cls[+self];
    cls[-self];
    cls[~self];
}

valueP js_object::defaultValue(PreferredType::Hint hint)
{
    valueP f1,f2;
    if(hint==PreferredType::Number || hint==PreferredType::NoHint) {
        f1=lookup("valueOf");
        f2=lookup("toString");
    }
    else {
        f1=lookup("toString");
        f2=lookup("valueOf");
    }
    valueP result;
    if(f1) result=f1();
    if(!result && f2) result=f2();
    return result;
}

boost::clipp::detail::converterP js_object::get_converter_from_this(const type_detail& to,precedence p,valueP& wrapped)
{
    boost::clipp::detail::converterP result=scope::get_converter_from_this(to,p,wrapped);
    if(!result) {
        const char* names[2]={"toString","valueOf"};
        for(int i=0;i<2;++i) {
            try {
                valueP function,primitive;
                if((function=lookup(names[i])) && 
                   (primitive=function()) && 
                   !unwrap<js_object>(primitive).ok()
                ) {
                    result=primitive->get_converter_from_this(to,p,primitive);
                    if(result) {
                        wrapped=primitive;
                        return result;
                    }
                }
            }
            catch(...) {}
        }
    }
    return result;
}

valueP boost::javascript::operator+(const js_object& lhs,valueP rhs)
{
    valueP lhs_result=toPrimitive(lhs);
    valueP rhs_result=toPrimitive(rhs);
    if(!lhs_result || isObject(lhs_result)) throw invalid_argument(bad_type_conversion("Object","Primitive"),1);
    if(!rhs_result || isObject(rhs_result)) throw invalid_argument(bad_type_conversion(lhs.get_context()->type_name(rhs->type()),"Primitive"),2);

    return invoke_operator<'+'>(lhs_result,rhs_result);
}

valueP boost::javascript::operator+(valueP lhs,const js_object& rhs)
{
    valueP lhs_result=toPrimitive(lhs);
    valueP rhs_result=toPrimitive(rhs);
    if(!rhs_result || isObject(lhs_result)) throw invalid_argument(bad_type_conversion(rhs.get_context()->type_name(lhs->type()),"Primitive"),1);
    if(!rhs_result || isObject(rhs_result)) throw invalid_argument(bad_type_conversion("Object","Primitive"),2);

    return invoke_operator<'+'>(lhs_result,rhs_result);
}

double boost::javascript::operator%(const js_object& lhs,double rhs)
{
    double value=math::floor(toNumber(lhs)/rhs);
    return lhs - double(value)*rhs;
}

double boost::javascript::operator%(double lhs,const js_object& rhs)
{
    double value=math::floor(lhs/toNumber(rhs));
    return lhs - double(value)*toNumber(rhs);
}
