// member_overloader.cpp: implementation of the member_overloader class.
//
//////////////////////////////////////////////////////////////////////

#include <boost/clipp/member_overloader.hpp>
#include <boost/clipp/member_property.hpp>
#include <stdexcept>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
using namespace boost::clipp;

member_overloader::member_overloader(member* rhs)
:   member(rhs->name().c_str())
,   type_(rhs->type())
{
    overloads_.push_back(rhs);
}

member_overloader::member_overloader()
:   member("")
{
}

type_detail member_overloader::type()
{
    return type_;
}

valueP member_overloader::join(valueP rhs)
{
    if(overloads_.size()==0) type_=rhs->type();
    if(rhs->type()==typeid(property_read_id) && type_==typeid(property_write_id)) {
        type_=typeid(property_id);
    }
    else if(rhs->type()==typeid(property_write_id) && type_==typeid(property_read_id)) {
        type_=typeid(property_id);
    }
    else if(type_==typeid(property_id) && (
        rhs->type()==typeid(property_write_id) ||
        rhs->type()==typeid(property_read_id)
    )
    );
    else if(rhs->type()!=type_) {
        throw std::runtime_error("Unable to overload type");
    }
    overloads_.push_back(rhs);
    return this;
}

valueP member_overloader::join_client(valueP rhs)
{
    valueP result=rhs;
    for(Overloads::iterator it=overloads_.begin();it!=overloads_.end();++it) {
        result=result->join(*it);
        if(!result) return 0;
    }
    return result;
}


valueP member_overloader::wrap_value(valueP parent) 
{
    valueP result = new member_wrapper(this,parent);
    if(type_==typeid(property_id)) {
        result = new member_property_wrapper(this,parent);
    }
    else {
        result = new member_wrapper(this,parent);
    }
    result->create(get_context());
    return result;    
}

valueP member_overloader::call(Params& args,valueP parent)
{
    for(Overloads::iterator it=overloads_.begin();it!=overloads_.end();++it) {
        valueP v=*it;
        if(v->validate_arguments(call_method,args,parent)) {
            try {
                return v->call(args,parent);
            }
            //Invalid type conversion inside function.
            catch(invalid_argument&) {}
            catch(invalid_number_of_arguments&) {}
        }
    }
    throw std::runtime_error("No appropriate overloads found");
}

valueP member_overloader::construct(Params& args,valueP parent)
{
    for(Overloads::iterator it=overloads_.begin();it!=overloads_.end();++it) {
        valueP v=*it;
        if(v->validate_arguments(construct_method,args,parent)) {
            try {
                return v->construct(args,parent);
            }
            //Invalid type conversion inside function.
            catch(invalid_argument&) {}
            catch(invalid_number_of_arguments&) {}
        }
    }
    throw std::runtime_error("No appropriate overloads found");
}

bool member_overloader::validate_arguments(value_method method,Params& args,valueP parent)
{
    for(Overloads::iterator it=overloads_.begin();it!=overloads_.end();++it) {
        valueP v=*it;
        if(v->validate_arguments(method,args,parent)) return true;
    }
    return false;
}

valueP member_overloader::lookup(const std::string& identifier,valueP parent)
{
    for(Overloads::iterator it=overloads_.begin();it!=overloads_.end();++it) {
        valueP v=*it;
        if(v->type()==typeid(property_read_id)) {
            return v->lookup(identifier,parent);
        }
    }
    return member::lookup(identifier,parent);
}

valueP member_overloader::duplicate(valueP parent)
{
    for(Overloads::iterator it=overloads_.begin();it!=overloads_.end();++it) {
        valueP v=*it;
        Params args;
        if(v->validate_arguments(duplicate_method,args,parent)) {
            return v->duplicate(parent);
        }
    }
    throw std::runtime_error("No appropriate overloads found");
}

valueP member_overloader::assign(valueP rhs,valueP parent)
{
    for(Overloads::iterator it=overloads_.begin();it!=overloads_.end();++it) {
        valueP v=*it;
        Params arguments;
        arguments.push_back(rhs);
        if(v->validate_arguments(assign_method,arguments,parent)) {
            valueP result=v->assign(rhs,parent);
            if(v->type()==typeid(property_write_id)) return this;
            else return result;
        }
    }
    throw std::runtime_error("No appropriate overloads found");
}

std::ostream& member_overloader::store(std::ostream& stream)
{
    for(Overloads::iterator it=overloads_.begin();it!=overloads_.end();++it) {
        valueP overload=*it;
        overload->store(stream);
    }
    return stream;
}
