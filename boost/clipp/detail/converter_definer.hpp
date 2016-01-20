// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_CONVERTER_DEFINER_HPP_HOLT_25102003
#define BOOST_CLIPP_CONVERTER_DEFINER_HPP_HOLT_25102003

#include <boost/clipp/converter.hpp>

namespace boost { namespace clipp { namespace detail {
    template<typename U>
    void wrap(U u) {
        detail::wrapper* wrapper = detail::define_wrapper(u,detail::get_signature(U,T),this);
        get_context()->add_wrapper(wrapper);
        
        detail::converter* converter = detail::define_converter_optional(detail::get_signature(U,T),this);
        if(converter) prototype()->add_converter_from_this(new detail::implicit_conversion_converter<from_traits,ToTraits>());

        get_context()->add_type_name(type_id<U>(),get_context()->type_name(typeid(T)));
    }
    template<typename U>
    void converter(U u) {
        detail::wrapper* wrapper = detail::define_wrapper_optional(u,detail::get_signature(U,T),this);
        if(wrapper) get_context()->add_converter_to_this(wrapper);

        detail::converter* converter = detail::define_converter_optional(detail::get_signature(U,T),this);
        if(converter) prototype()->add_converter_from_this(converter);
    }
    template<typename T>
    void register_class_converters(boost::type<T>const &t) {
        prototype()->add_converter_from_this(new detail::cast_converter<T>());
        get_context()->add_wrapper(new detail::cast_wrapper<T>());
        wrap(t);
    };
    void register_class_converters(boost::type<mpl::void_>) {
        prototype()->add_converter_from_this(new detail::cast_converter<T>());
        get_context()->add_wrapper(new detail::cast_wrapper<T>());
    };
    void register_bases(mpl::true_) {}
    void register_bases(mpl::false_) 
    {
        if(detail::register_type(boost::type<Bases>(),get_context())) {
            set_base_class(typeid(Bases));
        }
    }
private:
    template<typename U>
    void wrap(type<U> const& t) {
        typedef convertible_from_to<U,T>::type from_type;
        typedef convertible_to_from<U,T>::type to_type;

        wrap_helper(BOOST_CLIPP_UNWRAP_TYPE(from_type),mpl::not_<is_invalid_conversion<from_type> >());
        convert_helper(BOOST_CLIPP_UNWRAP_TYPE(to_type),mpl::not_<is_invalid_conversion<to_type> >());
        get_context()->add_type_name(type_id<U>(),get_context()->type_name(typeid(T)));
    }

    template<typename U>
    void converter(U u) {
        define_converter(u,detail::get_signature(u,boost::type<T>()));
    }
    template<typename FromTraits>
    void wrap_helper(FromTraits from,mpl::bool_<true>) {
        typedef mpl::list<T,detail::direct_,detail::cv_unqualified> to_traits;
        get_context()->add_wrapper(new detail::constructor_wrapper<FromTraits,to_traits>());
    }
    template<typename ToTraits>
    void convert_helper(ToTraits to,mpl::bool_<true>) {
        typedef mpl::list<T,detail::direct_,detail::cv_unqualified> from_traits;
        prototype()->add_converter_from_this(new detail::implicit_conversion_converter<from_traits,ToTraits>());
    }
    template<typename ToTraits>
    void convert_helper(ToTraits to,mpl::bool_<false>) {
        typedef detail::unwrap_value_type<ToTraits>::type::to_type input_type;
        BOOST_STATIC_ASSERT((is_same<input_type,Wrapped>::value==false));
        typedef convertible_to_from<Wrapped,T>::type wrapped_type;
        typedef convertible_to_from<input_type,wrapped_type>::type to_type;
        convert_helper_wrapped(BOOST_CLIPP_UNWRAP_TYPE(wrapped_type),BOOST_CLIPP_UNWRAP_TYPE(to_type),mpl::not_<is_invalid_conversion<to_type> >());
    }
    template<typename WrappedTraits,typename ToTraits>
    void convert_helper_wrapped(WrappedTraits wrapped,ToTraits to,mpl::bool_<true>) {
        typedef mpl::list<T,detail::direct_,detail::cv_unqualified> from_traits;
        prototype()->add_converter_from_this(new detail::implicit_conversion_wrapped_converter<from_traits,WrappedTraits,ToTraits>());
    }
    template<typename WrappedTraits,typename ToTraits>
    void convert_helper_wrapped(WrappedTraits wrapped,ToTraits to,mpl::bool_<false>) {
        unable_to_perform_conversion;
    }

    template<typename U,typename Signature>
    void define_converter(U u,Signature s) {
        detail::data_type<Signature>::type* dummy=NULL;
        define_converter(u,dummy,s);
    }
    template<typename U,typename Signature> 
    void define_converter(U u,detail::member_function_tag<T>*,Signature s) {
        BOOST_STATIC_ASSERT(detail::signature_arity<Signature>::value==0);
        typedef T from_type;
        typedef detail::return_type<Signature>::type to_type;
        define_function_call_converter(u,BOOST_CLIPP_UNWRAP_TYPE(from_type),BOOST_CLIPP_UNWRAP_TYPE(to_type));
    }
    template<typename U,typename Signature> 
    void define_converter(U u,detail::constructor_tag*,Signature s) {
        BOOST_STATIC_ASSERT(detail::signature_arity<Signature>::value==1);
        typedef detail::argument<Signature,0>::result_type from_type;
        typedef detail::return_type<Signature>::type to_type;
        define_constructor_converter(BOOST_CLIPP_UNWRAP_TYPE(from_type),BOOST_CLIPP_UNWRAP_TYPE(to_type));
    }
    template<typename Fn,typename FromTraits,typename ToTraits>
    void define_function_call_converter(Fn fn,FromTraits from,ToTraits to)
    {
        prototype()->add_converter_from_this(new detail::function_call_converter<FromTraits,ToTraits,Fn>(fn));
    }
    template<typename FromTraits,typename ToTraits>
    void define_constructor_converter(FromTraits from,ToTraits to)
    {
        if(is_same<T,detail::unwrap_value_type<FromTraits>::type>::value) {
            prototype()->add_converter_from_this(new detail::constructor_converter<FromTraits,ToTraits>());
            get_context()->add_wrapper(new detail::constructor_wrapper<FromTraits,ToTraits>());
        }
        else if(is_same<Wrapped,detail::unwrap_value_type<FromTraits>::type>::value) {
            get_context()->add_wrapper(new detail::constructor_wrapper<FromTraits,ToTraits>());
        }
        prototype()->add_converter_to_this(new detail::constructor_converter<FromTraits,ToTraits>());
    }
template<typename Signature,typename FromTraits,typename ToTraits,typename WrappedTraits>
struct generic_converter {
    typedef wrap_type<input_traits>::type           input_type;
    typedef wrap_type<ToTraits>::type               result_type;

    //Normal conversion
    result_type operator()(const valueP& input,valueP& output)
    {
        return execute(input,output,return_type_selector<Signature>::type,data_type<Signature>::type(input));
    }
    result_type execute(valueP const& input,valueP& output,boost::type<result_type>,member_function_tag<input_type>& ct)
    {
        return (ct().*t_)();
    }
    result_type execute(valueP const& input,valueP& output,boost::type<result_type>,free_function_tag&)
    {
        return t_(unwrap<input_type,p_unwrap>(input,output));
    }
    result_type execute(valueP const& input,valueP& output,boost::type<result_type>,member_data_tag<input_type>& ct)
    {
        return return ct();
    }
    result_type execute(valueP const& input,valueP& output,boost::type<result_type>,constructor_tag&)
    {
        result_type result=new to_type(unwrap<input_type>(input,p_function)());
        output=wrap_struct<result_type>::wrap(result,input->get_context());
        return result;
    }
    result_type execute(valueP const& input,valueP& output,boost::type<result_type>,implicit_conversion_tag<input_type>&)
    {
        return unwrap<input_type>(input,p_unwrap)();
    }
    result_type execute(valueP const& input,valueP& output,boost::type<wrapped_type>,implicit_conversion_tag<input_type>&)
    {
        output = wrap_struct<wrapped_type>::wrap(unwrap<wrapped_type>(input,p_function)(),input->get_context());
        return unwrap<result_type>(output,p_unwrap);
    }
    virtual precedence get_precedence() {
        return get_precedence_helper(type<data_type<Signature>::type>());
    }
    precedence get_precedence_helper(boost::type<member_function_tag<input_type> >) {return p_function;}
    precedence get_precedence_helper(boost::type<free_function_tag>) {return p_function;}
    precedence get_precedence_helper(boost::type<member_data_tag<input_type> >) {return p_unwrap;}
    precedence get_precedence_helper(boost::type<constructor_tag>) {return p_create;}
    precedence get_precedence_helper(boost::type<implicit_conversion_tag<input_type> >) {return p_unwrap;}
    precedence get_precedence_helper(boost::type<implicit_conversion_tag<wrapped_type> >) {return p_unwrap;}

    virtual type_detail from_type_info()    {return type_id<input_type>();}
private:
    T t_;
};

template<typename FromTraits,typename ToTraits,typename Fn>
struct function_call_converter 
:   converter_base_selector<
        unwrap_indirection<ToTraits>::type,
        unwrap_cv<ToTraits>::type
    >::inner<function_call_converter,wrap_type<ToTraits>::type>
{
    typedef unwrap_value_type<ToTraits>::type       to_type;
    typedef unwrap_value_type<FromTraits>::type     from_type;

    typedef mpl::list<from_type,reference_,unwrap_cv<FromTraits>::type>::type input_traits;

    typedef wrap_type<input_traits>::type           input_type;
    typedef wrap_type<ToTraits>::type               result_type;

    function_call_converter(Fn fn) : fn_(fn) {}
    virtual ~function_call_converter() {}

    result_type operator()(const valueP& input,valueP& output)
    {
        return (unwrap<input_type>(input,p_unwrap)().*fn_)();
    }

    virtual precedence get_precedence()                 {return p_function;}
    virtual type_detail from_type_info()    {return type_id<input_type>();}
private:
    Fn fn_;
};

template<typename FromTraits,typename ToTraits>
struct implicit_conversion_converter 
:   converter_base_selector<
        unwrap_indirection<ToTraits>::type,
        unwrap_cv<ToTraits>::type
    >::inner<implicit_conversion_converter,wrap_type<ToTraits>::type>
{
    typedef unwrap_value_type<ToTraits>::type       to_type;
    typedef unwrap_value_type<FromTraits>::type     from_type;

    typedef mpl::list<from_type,reference_,unwrap_cv<FromTraits>::type>::type input_traits;

    typedef wrap_type<input_traits>::type           input_type;
    typedef wrap_type<ToTraits>::type               result_type;

    implicit_conversion_converter() {}
    virtual ~implicit_conversion_converter() {}

    result_type operator()(const valueP& input,valueP& output)
    {
        return unwrap<input_type>(input,p_unwrap)();
    }

    virtual precedence get_precedence()                 {return p_function;}
    virtual type_detail from_type_info()    {return type_id<input_type>();}
};

template<typename FromTraits,typename WrappedTraits,typename ToTraits>
struct implicit_conversion_wrapped_converter 
:   converter_base_selector<
        unwrap_indirection<ToTraits>::type,
        unwrap_cv<ToTraits>::type
    >::inner<implicit_conversion_wrapped_converter,wrap_type<ToTraits>::type>
{
    typedef unwrap_value_type<ToTraits>::type       to_type;
    typedef unwrap_value_type<FromTraits>::type     from_type;

    typedef mpl::list<from_type,reference_,unwrap_cv<FromTraits>::type>::type input_traits;

    typedef wrap_type<input_traits>::type           input_type;
    typedef wrap_type<ToTraits>::type               result_type;
    typedef wrap_type<WrappedTraits>::type          wrapped_type;

    implicit_conversion_wrapped_converter() {}
    virtual ~implicit_conversion_wrapped_converter() {}

    result_type operator()(const valueP& input,valueP& output)
    {
        return wrapped_type(unwrap<input_type>(input,p_unwrap)());
    }

    virtual precedence get_precedence()                 {return p_function;}
    virtual type_detail from_type_info()    {return type_id<input_type>();}
};

template<typename FromTraits,typename ToTraits>
struct constructor_converter 
:   converter_base_selector<
        pointer_,
        cv_unqualified
    >::inner<constructor_converter,unwrap_value_type<ToTraits>::type*>

{
    typedef unwrap_value_type<ToTraits>::type           to_type;
    typedef unwrap_value_type<FromTraits>::type         from_type;

    typedef wrap_type<FromTraits>::type                 input_type;
    typedef unwrap_value_type<ToTraits>::type*          result_type;
    constructor_converter() {}
    virtual ~constructor_converter() {}
    result_type operator()(const valueP& input,valueP& output)
    {
        result_type result=new to_type(unwrap<input_type>(input,p_function)());
        output=wrap_struct<result_type>::wrap(result,input->get_context());
        return result;
    }

    virtual precedence get_precedence()                 {return p_create;}
    virtual type_detail from_type_info()    {return type_id<input_type>();}
};

template<typename To>
struct cast_converter
:   converter_base_selector<
        pointer_,
        cv_unqualified
    >::inner<cast_converter,To*>
{
    typedef To           to_type;
    typedef to_type*     result_type;
    cast_converter() {}
    virtual ~cast_converter() {}
    result_type operator()(const valueP& input,valueP& output)
    {
        return static_cast<result_type>(input.get());
    }

    virtual precedence get_precedence()                 {return p_unwrap;}
};

template<typename Fn,typename Wrapper,typename ToTraits>
struct unwrap_converter
:   converter_base_selector<
        unwrap_indirection<ToTraits>::type,
        unwrap_cv<ToTraits>::type
    >::inner<unwrap_converter,wrap_type<ToTraits>::type>        
{
    typedef unwrap_value_type<ToTraits>::type       to_type;
    typedef Wrapper                                 wrapper_type;

    typedef wrap_type<ToTraits>::type               result_type;

    unwrap_converter(Fn fn) : fn_(fn) {}
    virtual ~unwrap_converter() {}

    result_type operator()(const valueP& input,valueP& output)
    {
        return (static_cast<wrapper_type*>(input.get())->*fn_)();
    }
    virtual precedence get_precedence()                 {return p_unwrap;}
private:
    Fn fn_;
};


template<typename Signature,typename Class>
struct converter_signature
{
    template<typename U>
    converter* define(U u) {
    }
};

template<typename Signature,typename Class>
{
    template<typename U>
    converter* define(U u) {return NULL;}
}

template<typename U,typename Signature,typename Class>
converter* define_converter_optional(Signature,Class* cls)
{

    typedef if_<
        is_converter_signature<Signature,Class>,

        converter_signature<Signature,Class>,
        no_converter
    >
}


}}} //namespace boost::clipp::detail

#endif //BOOST_CLIPP_CONVERTER_DEFINER_HPP_HOLT_25102003