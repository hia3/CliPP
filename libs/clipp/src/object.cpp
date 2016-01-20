// object.cpp: implementation of the object class.
//
//////////////////////////////////////////////////////////////////////

#include <boost/clipp/object.hpp>
#include <boost/clipp/context.hpp>
#include <boost/clipp/member_table.hpp>
#include <boost/clipp/class.hpp>
#include <boost/clipp/member_property_wrapper.hpp>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

using namespace boost::clipp;

object::object()
:   member_table_(nullptr)
{
}

object::~object()
{
}

valueP object::prototype()
{
    return member_table_;
}

void object::create(context* c)
{
    value::create(c);
    member_table_=c->get_prototype(type());
}

detail::converterP object::get_converter_from_this(const type_detail& to,precedence p,valueP& wrapped)
{
    return prototype()->get_converter_from_this(to,p,wrapped);
}

valueP object::operator[](const std::string& identifier)
{
    return lookup(identifier,this);
}

valueP object::construct(Params& args,valueP parent)
{
    return this;
}

valueP object::lookup(const std::string& identifier,valueP parent)
{
    valueP result = value::lookup(identifier,this);
    if(result) {
        return result->wrap_value(this);
    }
    return result;
}

void object::init(context* c)
{
    if(is_type_defined<object>(c)) return;
    class_<object> cls("object",c);
    cls.operator_(self[std::string()]);
    cls.function("prototype",&object::prototype);
}
