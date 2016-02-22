// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_MEMBER_FUNCTION_HPP_HOLT_03082003
#define BOOST_CLIPP_MEMBER_FUNCTION_HPP_HOLT_03082003

#include <boost/clipp/member_with_arguments.hpp>
#include <boost/clipp/static_with_arguments.hpp>
#include <boost/clipp/argument_lists.hpp>
#include <boost/clipp/context.hpp>

#include <boost/clipp/detail/call_generator.hpp>
#include <boost/clipp/detail/argument_validator.hpp>
#include <boost/clipp/detail/register_types.hpp>

namespace boost { namespace clipp {

struct function_id {};

template<typename Signature> 
class member_function : public member_with_arguments<Signature, member_function<Signature> > {
public:
    typedef member_with_arguments<Signature, member_function> base_type;
    typedef detail::signature_type<Signature> value_type;
    member_function(const char* name,value_type function) 
    :   base_type(name)
    ,   function_(function) {}

    std::string format() {
        typedef detail::return_type<Signature> return_type;
        std::string format = this->get_context()->type_name(type_id<return_type>());
        return format + " " + this->name()+"("+this->format_argument_list()+")";
    }

    virtual bool validate_arguments(value::value_method method,Params& arguments,valueP parent) 
    {
        return method&value::call_method &&
               this->invoke(member_argument_list<member_function>(*this,arguments,parent),detail::argument_validator());
    }
    virtual valueP call(Params& arguments,valueP parent) 
    {
        return this->invoke(member_argument_list<member_function>(*this,arguments,parent),detail::call_generator<value_type>(function_));
    }
    virtual bool is_free() const {
        return false;
    }
    virtual type_detail type() {return typeid(function_id);}
    virtual std::ostream& store(std::ostream& stream) {
        typedef detail::return_type<Signature> return_type;
        stream << this->get_context()->type_name(type_id<return_type>());
        return stream << " " << this->name() << "(" << this->format_argument_list() << ");" << std::endl;
    }
private:
    value_type function_;
};

template<typename Signature>
class member_static_function : public static_with_arguments<Signature,member_static_function<Signature> > {
public:
    typedef static_with_arguments<Signature,member_static_function> base_type;
    typedef detail::signature_type<Signature> value_type;
    member_static_function(const char* name,value_type function) 
    :   base_type(name)
    ,   function_(function) {}

    std::string format() {        
        typedef detail::return_type<Signature> return_type;
        std::string format = this->get_context()->type_name(type_id<return_type>());
        return format + " " + this->name()+"("+ this->format_argument_list()+")";
    }

    virtual bool   validate_arguments(value::value_method method,Params& arguments,valueP parent) 
    {
        return method&value::call_method &&
               this->invoke(static_argument_list<member_static_function>(*this,arguments,parent),detail::argument_validator());
    }
    virtual valueP call(Params& arguments,valueP parent) 
    {
        return this->invoke(static_argument_list<member_static_function>(*this,arguments,parent),detail::call_generator<value_type>(function_));
    }
    virtual bool is_free() const {
        return true;
    }
    virtual type_detail type() {return typeid(function_id);}
    virtual std::ostream& store(std::ostream& stream) {
        typedef detail::return_type<Signature> return_type;
        stream << this->get_context()->type_name(type_id<return_type>());
        return stream << " " << this->name() << "(" << this->format_argument_list() << ");" << std::endl;
    }
private:
    value_type function_;
};

}} // namespace boost::clipp

#endif //BOOST_CLIPP_MEMBER_FUNCTION_HPP_HOLT_03082003
