#include <boost/clipp/member_wrapper.hpp>
#include <boost/clipp/unwrap.hpp>
#include <boost/clipp/class.hpp>

using namespace boost::clipp;

member_wrapper::member_wrapper(const valueP& value,const valueP& parent)
:   value_(value)
,   parent_(parent)
{
}

valueP member_wrapper::lookup(const std::string& identifier,valueP parent)
{
    valueP method = object::lookup(identifier,parent_);
    if(method) return method;
    return value_->lookup(identifier,parent_);
}

valueP member_wrapper::call(Params& args,valueP parent)
{
    return value_->call(args,parent_);
}

valueP member_wrapper::construct(Params& args,valueP parent)
{
    return value_->construct(args,parent_);
}

valueP member_wrapper::duplicate(valueP parent)
{
    return value_->duplicate(parent_);
}

valueP member_wrapper::assign(valueP rhs,valueP parent)
{
    return value_->assign(rhs,parent_);
}

bool member_wrapper::validate_arguments(value_method method,Params& args,valueP parent)
{
    return value_->validate_arguments(method,args,parent_);
}

detail::converterP member_wrapper::get_converter_from_this(const type_detail& to,precedence p,valueP& wrapped)
{    
    valueP direct = value_->duplicate(parent_);
    detail::converterP c=direct->get_converter_from_this(to,p,wrapped);
    if(c) {
        wrapped=direct;
        return c;
    }
    c = value_->get_converter_from_this(to,p,wrapped);
    if(c) {
        wrapped=value_;
        return c;
    }
    return object::get_converter_from_this(to,p,wrapped);
}

void member_wrapper::init(context* c)
{
    if(is_type_defined<member_wrapper>(c)) return;
    class_<member_wrapper> cls("__member_wrapper__",c);
}

valueP member_wrapper::join_client(valueP rhs)
{
    return value_->join_client(rhs);
}
