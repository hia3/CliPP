// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_MEMBER_CONSTRUCTOR_HPP_HOLT_23082003
#define BOOST_CLIPP_MEMBER_CONSTRUCTOR_HPP_HOLT_23082003

#include <boost/clipp/member.hpp>
#include <boost/clipp/detail/signature.hpp>
#include <boost/clipp/detail/signature_extracter.hpp>
#include <boost/clipp/detail/constructor_generator.hpp>
#include <boost/clipp/detail/argument_validator.hpp>
#include <boost/clipp/static_with_arguments.hpp>

namespace boost { namespace clipp {

struct member_constructor_id {};

template<typename Signature>
class member_constructor : public static_with_arguments<Signature,member_constructor<Signature> > {
public:
    typedef typename static_with_arguments<Signature,member_constructor> base_type;
    BOOST_STATIC_CONSTANT(int, arity = detail::signature_arity<Signature>::value);
    typedef detail::signature_type<Signature> value_type;
    member_constructor(const char* name,value_type constructor) 
    :   base_type(name)
    ,   constructor_(constructor) 
    {
        method_=value::value_method(value::call_method|value::construct_method);
    }
    member_constructor(const char* name,value_type constructor,value::value_method method) 
    :   base_type(name)
    ,   constructor_(constructor) 
    ,   method_(method) 
    {}

    virtual type_detail type() {return typeid(member_constructor_id);}

    virtual bool   validate_arguments(value::value_method method,Params& arguments,valueP parent = nullptr) {
        return method&(method_) &&
               invoke(static_argument_list<member_constructor>(*this,arguments,parent),detail::argument_validator());
    }
    virtual valueP construct(Params& arguments,valueP parent = nullptr){
        if(!(method_&construct_method)) return nullptr;
        return invoke(static_argument_list<member_constructor>(*this,arguments,parent),detail::constructor_generator<value_type>(constructor_));
    }
    virtual valueP call(Params& arguments,valueP parent = nullptr){
        if(!(method_&call_method)) return nullptr;
        return invoke(static_argument_list<member_constructor>(*this,arguments,parent),detail::constructor_generator<value_type>(constructor_));
    }
    virtual bool is_free() const {
        return true;
    }
    virtual std::ostream& store(std::ostream& stream) {
        typedef detail::return_type<Signature> return_type;
        stream << get_context()->type_name(type_id<return_type>());
        return stream << "(" << format_argument_list() << ");" << std::endl;
    }
private:
    value_type constructor_;
    value::value_method method_;
};

}} // namespace boost::clipp

#endif //BOOST_CLIPP_MEMBER_CONSTRUCTOR_HPP_HOLT_23082003
