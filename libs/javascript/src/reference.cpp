#include <boost/javascript/reference.hpp>
#include <boost/clipp/class.hpp>
#include <boost/clipp/detail/operator_invoker.hpp>

using namespace boost::javascript;
using namespace boost::clipp;

void reference::init(context* c)
{
    if(is_type_defined<reference>(c)) return;
    class_<reference,object> cls("reference",c);
    cls[self = valueP()];
    cls[self *=valueP()];
    cls[self %=valueP()];
    cls[self /=valueP()];
    cls[self +=valueP()];
    cls[self -=valueP()];
    cls[self <<=valueP()];
    cls[self >>=valueP()];
    cls[self &=valueP()];
    cls[self ^=valueP()];
    cls[self |=valueP()];
    cls[--self];
    cls[++self];
    cls[self--];
    cls[self++];
}

reference::reference(reference const& rhs)
:   object()
,   value_(rhs.value_)
{
}

reference::reference(valueP const& rhs)
:   value_(rhs)
{}

void reference::create(context* c)
{
    if(value_ && !value_->created()) value_->create(c);
    object::create(c);
}

boost::clipp::detail::converterP reference::get_converter_from_this(const type_detail& to,precedence p,valueP& wrapped)
{
    if(value_) {
        boost::clipp::detail::converterP c=value_->get_converter_from_this(to,p,wrapped);
        if(c) {
            wrapped=value_;
            return c;
        }
        c=object::get_converter_from_this(to,p,wrapped);
        if(!c) wrapped=value_; //This gives correct error messages: Cannot convert from array to set<double>.
        return c;
    }
    else {
        return object::get_converter_from_this(to,p,wrapped);
    }
}

valueP reference::duplicate(valueP parent)
{
    return value_;
}

valueP reference::assign(valueP rhs,valueP parent)
{
    value_=rhs;
    return this;
}

valueP reference::call(Params& args,valueP parent)
{
    if(parent && value_) {
        return value_->wrap_value(parent)->call(args);
    }
    else if(value_) return value_->call(args,parent);
    else return value::call(args,parent);
}

valueP reference::construct(Params& args,valueP parent)
{
    if(parent && value_) {
        return value_->wrap_value(parent)->construct(args);
    }
    else if(value_) return value_->construct(args,parent);
    else return value::construct(args,parent);
}

value_iterator reference::begin() const
{
    if(value_) return value_->begin();
    else return value::begin();
}

value_iterator reference::end() const
{
    if(value_) return value_->end();
    else return value::end();
}

size_t reference::size() const
{
    if(value_) return value_->size();
    else return value::size();
}

valueP reference::lookup(const std::string& identifier,valueP parent)
{
    valueP result;
    if(value_) result = value_->lookup(identifier,parent);
    if(!result) result = object::lookup(identifier,parent);
    return result;
}

reference& reference::operator=(valueP value)
{
    value_=value;
    return *this;
}
reference& reference::operator*=(valueP value)
{
    assign(invoke_operator<'*'>(this,value));
    return *this;
}

reference& reference::operator%=(valueP value)
{
    assign(invoke_operator<'%'>(this,value));
    return *this;
}

reference& reference::operator/=(valueP value)
{
    assign(invoke_operator<'/'>(this,value));
    return *this;
}

reference& reference::operator+=(valueP value)
{
    assign(invoke_operator<'+'>(this,value));
    return *this;
}

reference& reference::operator-=(valueP value)
{
    assign(invoke_operator<'-'>(this,value));
    return *this;
}

reference& reference::operator<<=(valueP value)
{
    assign(invoke_operator<'<<'>(this,value));
    return *this;
}

reference& reference::operator>>=(valueP value)
{
    assign(invoke_operator<'>>'>(this,value));
    return *this;
}

reference& reference::operator&=(valueP value)
{
    assign(invoke_operator<'&'>(this,value));
    return *this;
}

reference& reference::operator^=(valueP value)
{
    assign(invoke_operator<'^'>(this,value));
    return *this;
}

reference& reference::operator|=(valueP value)
{
    assign(invoke_operator<'|'>(this,value));
    return *this;
}


reference& reference::operator--()
{
    assign(invoke_operator<'-'>(this,wrap(1,get_context())));
    return *this;
}

reference& reference::operator++()
{
    assign(invoke_operator<'+'>(this,wrap(1,get_context())));
    return *this;
}

valueP reference::operator--(int)
{
    valueP value = duplicate();
    assign(invoke_operator<'-'>(this,wrap(1,get_context())));
    return value;
}

valueP reference::operator++(int)
{
    valueP value = duplicate();
    assign(invoke_operator<'+'>(this,wrap(1,get_context())));
    return value;
}

valueP reference::wrap_value(valueP parent)
{
   valueP result=new reference_wrapper(this,parent);
   result->create(get_context());
   return result;
}

reference_wrapper::reference_wrapper(valueP value,valueP parent)
:   value_(value)
,   parent_(parent)
{}

reference_wrapper::~reference_wrapper()
{}

valueP reference_wrapper::duplicate(valueP parent)
{
    return value_->duplicate(parent_);
}

valueP reference_wrapper::assign(valueP rhs,valueP parent)
{
    return value_->assign(rhs,parent_);
}

valueP reference_wrapper::call(Params& args,valueP parent)
{
    return value_->call(args,parent_);
}

valueP reference_wrapper::construct(Params& args,valueP parent)
{
    return value_->construct(args,parent_);
}

valueP reference_wrapper::lookup(const std::string& identifier,valueP parent)
{
    return value_->lookup(identifier,parent_);
}

detail::converterP reference_wrapper::get_converter_from_this(const type_detail& to,precedence p,valueP& wrapped)
{
    valueP post_destroy=wrapped;
    wrapped=value_;
    return value_->get_converter_from_this(to,p,wrapped);
}

value_iterator reference_wrapper::begin() const
{
    return value_->begin();
}

value_iterator reference_wrapper::end() const
{
    return value_->end();
}

size_t         reference_wrapper::size() const
{
    return value_->size();
}

void reference_wrapper::create(context* c)
{
    if(!value_->created()) value_->create(c);
    object::create(c);
}

valueP reference_wrapper::join_client(valueP rhs)
{
    return value_->join_client(rhs);   
}
