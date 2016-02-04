// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_OPERATOR_INVOKER_HPP_HOLT_25092003
#define BOOST_CLIPP_OPERATOR_INVOKER_HPP_HOLT_25092003

#include <boost/clipp/detail/operators.hpp>
#include <boost/clipp/member_overloader.hpp>
#include <stdexcept>

namespace boost { namespace clipp {

inline std::string translateId(int id) {
    union {
        char text[5];
        int id;
    } string_translator;
    string_translator.id=id;
    string_translator.text[4]=0;
    std::string op(string_translator.text);
    std::reverse(op.begin(),op.end());
    return op;
}

class operator_error : public std::runtime_error
{
public:
    static std::string get_expression(operator_id id,valueP lhs,valueP rhs) {
        context* c = nullptr;
        if(lhs && lhs->get_context()) c=lhs->get_context();
        else if(rhs && rhs->get_context()) c=rhs->get_context();

        std::string text;
        if(lhs) {
            if(c) text += c->type_name(lhs->type());
            else  text += typeid(lhs->type()).name();
        }
        else text += "null";
        text+=" " + translateId(id) + " ";
        if(rhs) {
            if(c) text += c->type_name(rhs->type());
            else  text += typeid(rhs->type()).name();
        }
        else text += "null";
        return text;
    }
    operator_error(operator_id id,valueP lhs,valueP rhs) 
    : std::runtime_error("Unable to resolve operator: " + get_expression(id,lhs,rhs))
    {}
};


//binary operator

inline valueP call_operator(valueP const& method,valueP const& lhs,valueP const& rhs)
{
    value::Params params(2);
    params[0]=lhs;
    params[1]=rhs;
    return method->call(params);
}

template<operator_id id>
valueP get_binary_operator(valueP const& lhs,valueP const& rhs,value::Params& params,boost::mpl::int_<id>* dummy = nullptr)
{
    params[0]=lhs;
    params[1]=rhs;

    valueP method1=lhs->lookup(detail::operator_l<id>::name(),lhs);
    valueP method2=rhs->lookup(detail::operator_r<id>::name(),rhs);
    if(method1 && method2) {
        valueP overload_resolution=new member_overloader();
        method1->join_client(overload_resolution);
        method2->join_client(overload_resolution);
        return overload_resolution;
    }
    else if(method1) return method1;
    else if(method2) return method2;
    else return nullptr;
}

template<operator_id id>
valueP get_binary_inplace_operator(valueP const& lhs,valueP const& rhs,boost::mpl::int_<id>* dummy = nullptr)
{
    value::Params params(2);
    params[0]=lhs;
    params[1]=rhs;

    valueP method = lhs ? lhs->lookup(detail::operator_l<id>::name(),lhs) : nullptr;
    if(method && method->validate_arguments(value::call_method,params)) return method;
    return nullptr;
}

template<operator_id id>
struct binary_inplace_operator_invoker
{
    static valueP invoke(valueP const& lhs,valueP const& rhs) {
        valueP method;
        if(method=get_binary_inplace_operator<id>(lhs,rhs)) return call_operator(method,lhs,rhs);
        throw operator_error(id,lhs,rhs);
    }
};

template<operator_id id>
struct binary_operator_invoker
{
    static valueP invoke(valueP const& lhs,valueP const& rhs) {
        valueP method;
        value::Params params(2);        
        if(method=get_binary_operator<id>(lhs,rhs,params)) return method->call(params);
        throw operator_error(id,lhs,rhs);
    }
};

template<> struct binary_operator_invoker<'='> : binary_inplace_operator_invoker<'='> {};
template<> struct binary_operator_invoker<'+='> : binary_inplace_operator_invoker<'+='> {};
template<> struct binary_operator_invoker<'-='> : binary_inplace_operator_invoker<'-='> {};
template<> struct binary_operator_invoker<'*='> : binary_inplace_operator_invoker<'*='> {};
template<> struct binary_operator_invoker<'/='> : binary_inplace_operator_invoker<'/='> {};
template<> struct binary_operator_invoker<'%='> : binary_inplace_operator_invoker<'%='> {};
template<> struct binary_operator_invoker<'<<='> : binary_inplace_operator_invoker<'<<='> {};
template<> struct binary_operator_invoker<'>>='> : binary_inplace_operator_invoker<'>>='> {};
template<> struct binary_operator_invoker<'&='> : binary_inplace_operator_invoker<'&='> {};
template<> struct binary_operator_invoker<'^='> : binary_inplace_operator_invoker<'^='> {};
template<> struct binary_operator_invoker<'|='> : binary_inplace_operator_invoker<'|='> {};
template<> struct binary_operator_invoker<'[]'> : binary_inplace_operator_invoker<'[]'> {};

template<>
struct binary_operator_invoker<'>'>
{
    static valueP invoke(valueP const& lhs,valueP const& rhs) {
        valueP method;
        value::Params params(2);
        if(method=get_binary_operator<'>'>(lhs,rhs,params)) return method->call(params);
        if(method=get_binary_operator<'<'>(rhs,lhs,params)) return method->call(params);
        if(method=get_binary_operator<'<='>(lhs,rhs,params)) return wrap(!unwrap<bool>(method->call(params))(),lhs->get_context());
        if(method=get_binary_operator<'>='>(rhs,lhs,params)) return wrap(!unwrap<bool>(method->call(params))(),lhs->get_context());

        throw operator_error('>',lhs,rhs);
    }
};

template<>
struct binary_operator_invoker<'<'>
{
    static valueP invoke(valueP const& lhs,valueP const& rhs) {
        valueP method;
        value::Params params(2);
        if(method=get_binary_operator<'<'>(lhs,rhs,params)) return method->call(params);
        if(method=get_binary_operator<'>'>(rhs,lhs,params)) return method->call(params);
        if(method=get_binary_operator<'>='>(lhs,rhs,params)) return wrap(!unwrap<bool>(method->call(params))(),lhs->get_context());
        if(method=get_binary_operator<'<='>(rhs,lhs,params)) return wrap(!unwrap<bool>(method->call(params))(),lhs->get_context());

        throw operator_error('<',lhs,rhs);
    }
};

template<>
struct binary_operator_invoker<'<='>
{
    static valueP invoke(valueP const& lhs,valueP const& rhs) {
        valueP method;
        value::Params params(2);
        if(method=get_binary_operator<'<='>(lhs,rhs,params)) return method->call(params);
        if(method=get_binary_operator<'>='>(rhs,lhs,params)) return method->call(params);
        if(method=get_binary_operator<'>'>(lhs,rhs,params)) return wrap(!unwrap<bool>(method->call(params))(),lhs->get_context());
        if(method=get_binary_operator<'<'>(rhs,lhs,params)) return wrap(!unwrap<bool>(method->call(params))(),lhs->get_context());

        throw operator_error('<=',lhs,rhs);
    }
};

template<>
struct binary_operator_invoker<'>='>
{
    static valueP invoke(valueP const& lhs,valueP const& rhs) {
        valueP method;
        value::Params params(2);
        if(method=get_binary_operator<'>='>(lhs,rhs,params)) return method->call(params);
        if(method=get_binary_operator<'<='>(rhs,lhs,params)) return method->call(params);
        if(method=get_binary_operator<'<'>(lhs,rhs,params)) return wrap(!unwrap<bool>(method->call(params))(),lhs->get_context());
        if(method=get_binary_operator<'>'>(rhs,lhs,params)) return wrap(!unwrap<bool>(method->call(params))(),lhs->get_context());

        throw operator_error('>=',lhs,rhs);
    }
};

template<>
struct binary_operator_invoker<'=='>
{
    static valueP invoke(valueP const& lhs,valueP const& rhs) {
        valueP method;
        value::Params params(2);
        if(method=get_binary_operator<'=='>(lhs,rhs,params)) return method->call(params);
        if(method=get_binary_operator<'=='>(rhs,lhs,params)) return method->call(params);
        if(method=get_binary_operator<'!='>(lhs,rhs,params)) return wrap(!unwrap<bool>(method->call(params))(),lhs->get_context());
        if(method=get_binary_operator<'!='>(rhs,lhs,params)) return wrap(!unwrap<bool>(method->call(params))(),rhs->get_context());

        throw operator_error('==',lhs,rhs);
    }
};

template<>
struct binary_operator_invoker<'!='>
{
    static valueP invoke(valueP const& lhs,valueP const& rhs) {
        valueP method;
        value::Params params(2);
        if(method=get_binary_operator<'!='>(lhs,rhs,params)) return method->call(params);
        if(method=get_binary_operator<'!='>(rhs,lhs,params)) return method->call(params);
        if(method=get_binary_operator<'=='>(lhs,rhs,params)) return wrap(!unwrap<bool>(method->call(params))(),lhs->get_context());
        if(method=get_binary_operator<'=='>(rhs,lhs,params)) return wrap(!unwrap<bool>(method->call(params))(),rhs->get_context());

        throw operator_error('!=',lhs,rhs);
    }
};

template<operator_id id>
valueP invoke_operator(valueP const& lhs,valueP const& rhs,boost::mpl::int_<id>* dummy = nullptr)
{
    return binary_operator_invoker<id>::invoke(lhs,rhs);
}

//postfix ++,--
template<operator_id id>
valueP invoke_operator(valueP const& lhs,int rhs,boost::mpl::int_<id>* dummy = nullptr)
{
    value::Params params(1);
    params[0]=lhs;
    valueP method=lhs->lookup(detail::operator_l<id>::name(),lhs);
    if(method && method->validate_arguments(value::call_method,params)) {
        return method->call(params);
    }
    std::string error = "Unable to resolve operator: ";
    error+=lhs->get_context()->type_name(lhs->type());
    error+=translateId(id);
    throw std::runtime_error(error.c_str());
}

template<operator_id id>
valueP invoke_operator(valueP const& lhs,boost::mpl::int_<id>* dummy = nullptr)
{
    value::Params params(1);
    params[0]=lhs;
    valueP method=lhs->lookup(detail::operator_1<id>::name());
    if(method && method->validate_arguments(value::call_method,params)) {
        return method->call(params);
    }
    std::string error = "Unable to resolve operator: ";
    error+=translateId(id);
    error+=lhs->get_context()->type_name(lhs->type());
    throw std::runtime_error(error.c_str());
}

}} //namespace boost::clipp


#endif //BOOST_CLIPP_OPERATOR_INVOKER_HPP_HOLT_25092003
