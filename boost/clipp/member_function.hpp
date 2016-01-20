// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_MEMBER_FUNCTION_HPP_HOLT_03082003
#define BOOST_CLIPP_MEMBER_FUNCTION_HPP_HOLT_03082003

#include <boost/clipp/member_with_arguments.hpp>
#include <boost/clipp/static_with_arguments.hpp>
#include "boost/clipp/argument_lists.hpp"
#include "boost/clipp/context.hpp"

#include "boost/clipp/detail/call_generator.hpp"
#include "boost/clipp/detail/argument_validator.hpp"
#include "boost/clipp/detail/register_types.hpp"

namespace boost { namespace clipp {

struct function_id {};

template<typename Signature>
class member_function : public member_with_arguments<detail::signature_arity<Signature>::value-1>::inner<Signature,member_function<Signature> > {
public:
    typedef typename member_with_arguments<detail::signature_arity<Signature>::value-1>::inner<Signature,member_function<Signature> > base_type;
    typedef typename detail::signature_type<Signature>::type value_type;
    member_function(const char* name,value_type function) 
    :   base_type(name)
    ,   function_(function) {}

    std::string format() {
        typedef typename detail::return_type<Signature>::type return_type;
        std::string format = get_context()->type_name(type_id<return_type>());
    return format + " " + name()+"("+format_argument_list()+")";
    }

    virtual bool validate_arguments(value::value_method method,Params& arguments,valueP parent) 
    {
        return method&call_method && 
               invoke(member_argument_list<member_function>(*this,arguments,parent),detail::argument_validator());
    }
    virtual valueP call(Params& arguments,valueP parent) 
    {
        return invoke(member_argument_list<member_function>(*this,arguments,parent),detail::call_generator<value_type>(function_));
    }
    virtual bool is_free() const {
        return false;
    }
    virtual type_detail type() {return typeid(function_id);}
    virtual std::ostream& store(std::ostream& stream) {
        typedef typename detail::return_type<Signature>::type return_type;
        stream << get_context()->type_name(type_id<return_type>());
        return stream << " " << name() << "(" << format_argument_list() << ");" << std::endl;        
    }
private:
    value_type function_;
};

template<typename Signature>
class member_static_function : public static_with_arguments<detail::signature_arity<Signature>::value>::inner<Signature,member_static_function<Signature> > {
public:
    typedef typename static_with_arguments<detail::signature_arity<Signature>::value>::inner<Signature,member_static_function<Signature> > base_type;
    typedef typename detail::signature_type<Signature>::type value_type;
    member_static_function(const char* name,value_type function) 
    :   base_type(name)
    ,   function_(function) {}

    std::string format() {        
        typedef typename detail::return_type<Signature>::type return_type;
        std::string format = get_context()->type_name(type_id<return_type>());
        return format + " " + name()+"("+format_argument_list()+")";
    }

    virtual bool   validate_arguments(value::value_method method,Params& arguments,valueP parent) 
    {
        return method&call_method && 
               invoke(static_argument_list<member_static_function>(*this,arguments,parent),detail::argument_validator());
    }
    virtual valueP call(Params& arguments,valueP parent) 
    {
        return invoke(static_argument_list<member_static_function>(*this,arguments,parent),detail::call_generator<value_type>(function_));
    }
    virtual bool is_free() const {
        return true;
    }
    virtual type_detail type() {return typeid(function_id);}
    virtual std::ostream& store(std::ostream& stream) {
        typedef typename detail::return_type<Signature>::type return_type;
        stream << get_context()->type_name(type_id<return_type>());
        return stream << " " << name() << "(" << format_argument_list() << ");" << std::endl;
    }
private:
    value_type function_;
};

/*
template<typename Fn>
valueP function(context* c,const std::string& name,Fn fn) {
    member* m = detail::define_function<static_argument_list>(fn,detail::get_signature(fn,boost::type<void>()));
    m->create(c);
    c->global()->insert(name,m);
    return m;
}

template<typename Fn>
member* function(Fn fn) {
    return detail::define_function<member_argument_list>(fn,detail::get_signature(fn,boost::type<void>()));
}

template<typename Fn>
member* static_function(Fn fn) {
    return detail::define_function<static_argument_list>(fn,detail::get_signature(fn,boost::type<void>()));
}
*/

}} // namespace boost::clipp

#endif //BOOST_CLIPP_MEMBER_FUNCTION_HPP_HOLT_03082003
