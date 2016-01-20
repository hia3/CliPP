#include <boost/clipp/member_property_wrapper.hpp>
#include <boost/clipp/class.hpp>
#include <boost/clipp/detail/operator_invoker.hpp>

using namespace boost::clipp;

void member_property_wrapper::init(context* c)
{
    if(is_type_defined<member_property_wrapper>(c)) return;
    class_<member_property_wrapper,member_wrapper> cls_("__property__",c);
    cls_[self = valueP()];
    cls_[self += valueP()];
    cls_[self -= valueP()];
    cls_[self *= valueP()];
    cls_[self /= valueP()];
    cls_[self <<= valueP()];
    cls_[self >>= valueP()];
    cls_[self &= valueP()];
    cls_[self ^= valueP()];
    cls_[self |= valueP()];
}

valueP member_property_wrapper::operator=(valueP rhs)
{
    return assign(rhs);
}

valueP member_property_wrapper::operator+=(valueP rhs)
{
    return assign(invoke_operator<'+'>(this,rhs));
}

valueP member_property_wrapper::operator-=(valueP rhs)
{
    return assign(invoke_operator<'-'>(this,rhs));
}

valueP member_property_wrapper::operator*=(valueP rhs)
{
    return assign(invoke_operator<'*'>(this,rhs));
}

valueP member_property_wrapper::operator/=(valueP rhs)
{
    return assign(invoke_operator<'/'>(this,rhs));
}

valueP member_property_wrapper::operator<<=(valueP rhs)
{
    return assign(invoke_operator<'<<'>(this,rhs));
}

valueP member_property_wrapper::operator>>=(valueP rhs)
{
    return assign(invoke_operator<'>>'>(this,rhs));
}

valueP member_property_wrapper::operator&=(valueP rhs)
{
    return assign(invoke_operator<'&'>(this,rhs));
}

valueP member_property_wrapper::operator^=(valueP rhs)
{
    return assign(invoke_operator<'^'>(this,rhs));
}

valueP member_property_wrapper::operator|=(valueP rhs)
{
    return assign(invoke_operator<'|'>(this,rhs));
}
