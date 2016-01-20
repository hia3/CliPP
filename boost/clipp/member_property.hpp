// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_MEMBER_PROPERTY_HPP_HOLT_15092003
#define BOOST_CLIPP_MEMBER_PROPERTY_HPP_HOLT_15092003

#include "boost/clipp/member.hpp"
#include "boost/clipp/object.hpp"
#include "boost/clipp/context.hpp"
#include "boost/clipp/argument_lists.hpp"
#include "boost/clipp/member_with_arguments.hpp"
#include "boost/clipp/static_with_arguments.hpp"
#include <boost/clipp/member_property_wrapper.hpp>

#include "boost/clipp/detail/signature_extracter.hpp"
#include "boost/clipp/detail/read_generator.hpp"
#include "boost/clipp/detail/write_generator.hpp"
#include "boost/clipp/detail/argument_validator.hpp"
#include "boost/clipp/detail/register_types.hpp"

#include "boost/mpl/size.hpp"
#include "boost/mpl/at.hpp"
#include "boost/mpl/int.hpp"

namespace boost { namespace clipp {

struct property_read_id {};
struct property_write_id {};
struct property_id {};

template<typename Signature>
class member_property_read : public member_with_arguments<Signature, member_property_read<Signature> >
{
public:
    static_assert(0 == detail::signature_arity<Signature>::value - 1, "wrong arity ");
    typedef member_with_arguments<Signature, member_property_read> base_type;
    typedef detail::signature_type<Signature> value_type;
    member_property_read(const char* name,value_type data) 
    :   member_with_arguments<Signature,member_property_read>(name)
    ,   data_(data) {}
    virtual ~member_property_read() {}

    virtual valueP duplicate(valueP parent = nullptr) {
        Params arguments;
        return call(arguments,parent);
    }
    virtual valueP call(Params& arguments,valueP parent = nullptr){
        return invoke(member_argument_list<member_property_read>(*this,arguments,parent),detail::read_generator<value_type>(data_));
    }
    virtual bool   validate_arguments(value::value_method method,Params& arguments,valueP parent = nullptr) {
        return method&(duplicate_method|call_method) && 
               invoke(member_argument_list<member_property_read>(*this,arguments,parent),detail::argument_validator());
    }
    valueP lookup(const std::string& identifier,valueP parent)
    {
        valueP result = duplicate(parent);
        if(result) return result->lookup(identifier,parent);
        else return nullptr;
    }
    virtual bool is_free() const {
        return false;
    }
    virtual detail::converterP get_converter_from_this(const type_detail& to,precedence p,valueP& wrapped) {
        valueP direct = duplicate();
        detail::converterP c=direct->get_converter_from_this(to,p,wrapped);
        if(c) {
            wrapped=direct;
            return c;
        }
        return base_type::get_converter_from_this(to,p,wrapped);
    }
    virtual valueP wrap_value(valueP parent) {
        valueP result = new member_property_wrapper(this,parent);
        result->create(get_context());
        return result;
    }

    virtual type_detail type() {return typeid(property_read_id);}
private:
    value_type data_;
};

template<typename Signature>
class static_property_read : public static_with_arguments<Signature,static_property_read<Signature> > 
{
public:
    typedef typename static_with_arguments<Signature,static_property_read> base_type;
    typedef detail::signature_type<Signature> value_type;
    static_property_read(const char* name,value_type data) 
    :   base_type(name)
    ,   data_(data) {}

    virtual valueP duplicate(valueP parent = nullptr) {
        Params arguments;
        return call(arguments,parent);
    }
    virtual valueP call(Params& arguments,valueP parent = nullptr){
        return invoke(static_argument_list<static_property_read>(*this,arguments,parent),detail::read_generator<value_type>(data_,&backup_));
    }
    virtual bool   validate_arguments(value::value_method method,Params& arguments,valueP parent = nullptr) {
        return method&(duplicate_method|call_method) && 
               invoke(static_argument_list<static_property_read>(*this,arguments,parent),detail::argument_validator());
    }
    valueP lookup(const std::string& identifier,valueP parent)
    {
        valueP result = duplicate(parent);
        if(result) return result->lookup(identifier,parent);
        else return nullptr;
    }
    virtual detail::converterP get_converter_from_this(const type_detail& to,precedence p,valueP& wrapped) {
        valueP direct = duplicate();
        detail::converterP c=direct->get_converter_from_this(to,p,wrapped);
        if(c) {
            wrapped=direct;
            return c;
        }
        return base_type::get_converter_from_this(to,p,wrapped);
    }
    virtual valueP wrap_value(valueP parent) {
        valueP result = new member_property_wrapper(this,parent);
        result->create(get_context());
        return result;
    }

    virtual type_detail type() {return typeid(property_read_id);}
    virtual bool is_free() const {return true;}
private:
    value_type data_;
    valueP backup_;
};

template<typename Signature>
class member_property_write : public member_with_arguments<Signature,member_property_write<Signature> >
{
public:
    static_assert(1 == detail::signature_arity<Signature>::value - 1, "wrong arity ");
    typedef member_with_arguments<Signature,member_property_write> base_type;
    typedef detail::signature_type<Signature> value_type;
    member_property_write(const char* name,value_type data) 
    :   base_type(name)
    ,   data_(data) {}
    virtual ~member_property_write() {}    
    virtual valueP assign(valueP rhs,valueP parent = nullptr) {
        Params arguments;
        arguments.push_back(rhs);
        return call(arguments,parent);
    }
    virtual valueP call(Params& arguments,valueP parent = nullptr){
        return invoke(member_argument_list<member_property_write>(*this,arguments,parent),detail::write_generator<value_type>(data_));
    }
    virtual bool   validate_arguments(value::value_method method,Params& arguments,valueP parent = nullptr) {
        return method&(assign_method|call_method) && 
               invoke(member_argument_list<member_property_write>(*this,arguments,parent),detail::argument_validator());
    }
    virtual valueP wrap_value(valueP parent) {
        valueP result = new member_property_wrapper(this,parent);
        result->create(get_context());
        return result;
    }

    virtual bool is_free() const {return false;}
    virtual type_detail type() {return typeid(property_write_id);}
private:
    value_type data_;
};

template<typename Signature>
class static_property_write : public static_with_arguments<Signature,static_property_write<Signature> > 
{
public:
    typedef typename static_with_arguments<Signature,static_property_write<Signature> > base_type;
    typedef detail::signature_type<Signature> value_type;
    static_property_write(const char* name,value_type data) 
    :   base_type(name)
    ,   data_(data) {}
    virtual ~static_property_write() {}    
    virtual valueP assign(valueP rhs,valueP parent = nullptr) {
        Params arguments;
        arguments.push_back(rhs);
        return call(arguments,parent);
    }
    virtual valueP call(Params& arguments,valueP parent = nullptr){
        return invoke(static_argument_list<static_property_write>(*this,arguments,parent),detail::write_generator<value_type>(data_));
    }
    virtual bool   validate_arguments(value::value_method method,Params& arguments,valueP parent = nullptr) {
        return method&(assign_method|call_method) && 
               invoke(static_argument_list<static_property_write>(*this,arguments,parent),detail::argument_validator());
    }
    virtual valueP wrap_value(valueP parent) {
        valueP result = new member_property_wrapper(this,parent);
        result->create(get_context());
        return result;
    }

    virtual bool is_free() const {return false;}
    virtual type_detail type() {return typeid(property_write_id);}
private:
    value_type data_;
};

/*template<typename U>
valueP read(context* c,const std::string& name,U u) {
    member* m = detail::define_read(u,detail::get_signature(u,boost::type<void>()));
    m->create(c);
    c->global()->insert(name,m);
    return m;
}

template<typename U>
valueP write(context* c,const std::string& name,U u) {
    member* m = detail::define_write(u,detail::get_signature(u,boost::type<void>()));
    m->create(c);
    c->global()->insert(name,m);
    return m;
}

template<typename Read,typename Write>
valueP read_write(context* c,const std::string& name,Read r,Write w) {
    read(c,name,r);
    return write(c,name,w);
}
*/
}} // namespace boost::clipp

#endif //BOOST_CLIPP_MEMBER_PROPERTY_HPP_HOLT_15092003

