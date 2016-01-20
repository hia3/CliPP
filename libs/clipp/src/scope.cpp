#include <boost/clipp/scope.hpp>
#include <boost/clipp/class.hpp>
#include <boost/clipp/value_iterator.hpp>
#include <boost/clipp/member_property_wrapper.hpp>
#include <boost/clipp/member_wrapper.hpp>

using namespace boost::clipp;

void scope::init(context* c)
{
    if(is_type_defined<scope>(c)) return;
    class_<scope,object> cls("scope",c);
}

scope::scope()
{}

scope::~scope()
{
    // remove values in the right order
    while (!stack_.empty())
    {
        values_.erase(stack_.back());
        stack_.pop_back();
    }
}

valueP scope::lookup(const std::string& identifier,valueP parent)
{
    NameMap::iterator it = values_.find(identifier);
    if(it!=values_.end()) {
        valueP result=it->second;
        return result->wrap_value(this);
    }
    else return object::lookup(identifier,parent);
}

valueP scope::insert(const std::string& identifier,valueP value)
{
    valueP& old = values_[identifier];
    //If identifier exists, this may be a method, in which case we need to create an overload.
    if(old) {
        old = old->join(value);
        //But if old is not a method, overwrite it.
        if(!old) old=value;
    }
    else {
        old = value;
        stack_.push_back(identifier);
    }
    return  old;
}

void scope::erase(valueP value)
{
    NameMap::iterator iter;
    for (iter = values_.begin(); iter != values_.end(); iter++)
    {
        if (iter->second->duplicate() == value->duplicate())
        {
            stack_.remove(iter->first);
            values_.erase(iter);
            break;
        }
    }
}

value_iterator scope::begin() const
{
    return value_iterator(values_.begin(),get_context());
}

value_iterator scope::end() const
{
    return value_iterator(values_.end(),get_context());
}

size_t scope::size() const
{
    return values_.size();
}

std::ostream& scope::store(std::ostream& stream)
{
    for(NameMap::iterator it=values_.begin();it!=values_.end();++it) {
        valueP object=it->second;
        object->store(stream);
    }
    return stream;
}

void scope::clear()
{
    values_.clear();
}
