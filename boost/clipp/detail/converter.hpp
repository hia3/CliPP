// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_CONVERTER_HPP_HOLT_03092003
#define BOOST_CLIPP_CONVERTER_HPP_HOLT_03092003

#include <boost/clipp/unwrap.hpp>
#include <boost/clipp/wrap.hpp>
#include <boost/clipp/ref_base.hpp>
#include <boost/clipp/decoration.hpp>
#include <boost/clipp/value_wrapper.hpp>

namespace boost { namespace clipp { namespace detail {

///Convert between different decorations
template<typename Converter,typename T>
struct redecorate_converter_base : converter<T>
{
    typedef Converter converter_type;
    typedef typename mpl::list<void,typename unwrap_type<T>::indirection,typename unwrap_type<T>::cv> new_decoration;
    redecorate_converter_base(Converter* c) : c_(c) {};
    virtual ~redecorate_converter_base() {}
    converterP get_converter(decoration d) {
        if(decoration(unwrap_decoration<new_decoration>::value)==d) return this;
        return c_->get_converter(d);
    }
    precedence get_precedence()                 {return c_->get_precedence();}
    type_detail from_type_info()                {return c_->from_type_info();}
    converter_type& other_converter() {
        return *c_;
    }
private:
    intrusive_ptr<converter_type> c_;
};

template<typename Converter,typename T>
struct address_of_converter : redecorate_converter_base<Converter,T>
{
    typedef typename converter<T>::result_type result_type;
    address_of_converter(Converter* c) : redecorate_converter_base<Converter,T>(c) {}
    virtual ~address_of_converter() {}
    result_type operator()(const valueP& input,valueP& output) {
        return &this->other_converter()(input,output);
    }
};

template<typename Converter,typename T>
struct dereference_converter : redecorate_converter_base<Converter,T>
{
    typedef typename converter<T>::result_type result_type;
    dereference_converter(Converter* c) : redecorate_converter_base<Converter,T>(c) {}
    virtual ~dereference_converter() {}
    result_type operator()(const valueP& input,valueP& output) {
        return *this->other_converter()(input,output);
    }
};

template<typename Converter,typename T>
struct redecorate_converter : redecorate_converter_base<Converter,T>
{
    typedef typename converter<T>::result_type result_type;
    redecorate_converter(Converter* c) : redecorate_converter_base<Converter,T>(c) {}
    virtual ~redecorate_converter() {}
    result_type operator()(const valueP& input,valueP& output) {
        return this->other_converter()(input,output);
    }
};

template<typename Converter,typename T>
struct dereference_wrap_converter : redecorate_converter_base<Converter,T>
{
    typedef typename converter<T>::result_type result_type;
    dereference_wrap_converter(Converter* c) : redecorate_converter_base<Converter,T>(c) {}
    virtual ~dereference_wrap_converter() {}
    virtual result_type operator()(const valueP& input,valueP& output) {
        value_wrapper<BOOST_DEDUCED_TYPENAME Converter::to_type>* wrapper=
            new value_wrapper<BOOST_DEDUCED_TYPENAME Converter::to_type>(this->other_converter()(input,output));
        output=wrapper;
        return *wrapper;
    }
};

/*
T -> 
    T
    T const
    T const&

T const ->
    T
    T const
    T const&

T& ->
    all

T const& ->
    T
    T const
    T const&
    T const*

T* ->
    all

T const* ->
    T
    T const
    T const&
    T const*

*/

template<typename indirection,typename cv>
struct converter_base_selector;

template<typename Converter,typename T>
struct decorated_converter_base : converter<T> 
{
    typedef Converter           converter_type;
    Converter& derived() {
        return static_cast<Converter&>(*this);
    }
};

template<>
struct converter_base_selector<direct_,cv_unqualified> 
{
    template<typename Converter,typename T>
    struct inner : decorated_converter_base<Converter,T> {
        converterP get_converter(decoration d) {
            typedef Converter converter_type;
            typedef typename converter_type::to_type  to_type;
            switch(d) {
            case decoration::direct:
                return this;
            case decoration::const_direct:
                return new redecorate_converter<converter_type,to_type const>(&this->derived());
            case decoration::const_reference:
                return new dereference_wrap_converter<converter_type,to_type const&>(&this->derived());
            default:
                return nullptr;
            }
        }
    };
};

template<>
struct converter_base_selector<direct_,const_> 
{
    template<typename Converter,typename T>
    struct inner : decorated_converter_base<Converter,T> {
        converterP get_converter(decoration d) {
            typedef Converter converter_type;
            typedef typename converter_type::to_type  to_type;
            switch(d) {
            case decoration::direct:
                return new redecorate_converter<converter_type,to_type>(&this->derived());
            case decoration::const_direct:
                return this;
            case decoration::const_reference:
                return new dereference_wrap_converter<converter_type,to_type const&>(&this->derived());
            default:
                return nullptr;
            }
        }
    };
};

template<>
struct converter_base_selector<pointer_,cv_unqualified> 
{
    template<typename Converter,typename T>
    struct inner : decorated_converter_base<Converter,T> {
        converterP get_converter(decoration d) {
            typedef Converter converter_type;
            typedef typename converter_type::to_type  to_type;
            switch(d) {
/*            case decoration::direct:
                return new dereference_converter<converter_type,to_type>(&this->derived());
            case decoration::const_direct:
                return new dereference_converter<converter_type,to_type const>(&this->derived());*/
            case decoration::pointer:
                return this;
            case decoration::const_pointer:
                return new redecorate_converter<converter_type,to_type const*>(&this->derived());
            case decoration::reference:
                return new dereference_converter<converter_type,to_type&>(&this->derived());
            case decoration::const_reference:
                return new dereference_converter<converter_type,to_type const&>(&this->derived());
            default:
                return nullptr;
            }
        }
    };
};

template<>
struct converter_base_selector<pointer_,const_> 
{
    template<typename Converter,typename T>
    struct inner : decorated_converter_base<Converter,T> {
        converterP get_converter(decoration d) {
            typedef Converter converter_type;
            typedef typename converter_type::to_type  to_type;
            switch(d) {
            case decoration::direct:
                return new dereference_converter<converter_type,to_type>(&this->derived());
            case decoration::const_direct:
                return new dereference_converter<converter_type,to_type const>(&this->derived());
            case decoration::const_reference:
                return new dereference_converter<converter_type,to_type const&>(&this->derived());
            case decoration::const_pointer:
                return this;
            default:
                return nullptr;
            }
        }
    };
};

template<>
struct converter_base_selector<reference_,cv_unqualified> 
{
    template<typename Converter,typename T>
    struct inner : decorated_converter_base<Converter,T> {
        converterP get_converter(decoration d) {
            typedef Converter converter_type;
            typedef typename converter_type::to_type  to_type;
            switch(d) {
            case decoration::direct:
                return new redecorate_converter<converter_type,to_type>(&this->derived());
            case decoration::const_direct:
                return new redecorate_converter<converter_type,to_type const>(&this->derived());
            case decoration::pointer:
                return new address_of_converter<converter_type,to_type*>(&this->derived());
            case decoration::const_pointer:
                return new address_of_converter<converter_type,to_type const*>(&this->derived());
            case decoration::reference:
                return this;
            case decoration::const_reference:
                return new redecorate_converter<converter_type,to_type const&>(&this->derived());
            default:
                return nullptr;
            }
        }
    };
};

template<>
struct converter_base_selector<reference_,const_> 
{
    template<typename Converter,typename T>
    struct inner : decorated_converter_base<Converter,T> {
        converterP get_converter(decoration d) {
            typedef Converter converter_type;
            typedef typename converter_type::to_type  to_type;
            switch(d) {
            case decoration::direct:
                return new redecorate_converter<converter_type,to_type>(&this->derived());
            case decoration::const_direct:
                return new redecorate_converter<converter_type,to_type const>(&this->derived());
            case decoration::const_reference:
                return this;
            case decoration::const_pointer:
                return new address_of_converter<converter_type,to_type const*>(&this->derived());
            default:
                return nullptr;
            }
        }
    };
};

template<typename FromTraits,typename ToTraits,typename Fn>
struct function_call_converter 
:   converter_base_selector<
        typename unwrap_indirection<ToTraits>::type,
        typename unwrap_cv<ToTraits>::type
    >::template inner<function_call_converter<FromTraits,ToTraits,Fn>,typename wrap_type<ToTraits>::type>
{
    typedef typename unwrap_value_type<ToTraits>::type       to_type;
    typedef typename unwrap_value_type<FromTraits>::type     from_type;

    typedef typename mpl::list<from_type,reference_,typename unwrap_cv<FromTraits>::type>::type input_traits;

    typedef typename wrap_type<input_traits>::type           input_type;
    typedef typename wrap_type<ToTraits>::type               result_type;

    function_call_converter(Fn fn) : fn_(fn) {}
    virtual ~function_call_converter() {}

    result_type operator()(const valueP& input,valueP& output)
    {
        return (unwrap<input_type>(input, precedence::unwrap)().*fn_)();
    }

    virtual precedence get_precedence()                 {return precedence::function;}
    virtual type_detail from_type_info()    {return type_id<input_type>();}
private:
    Fn fn_;
};

template<typename FromTraits,typename ToTraits>
struct implicit_conversion_converter 
:   converter_base_selector<
        typename unwrap_indirection<ToTraits>::type,
        typename unwrap_cv<ToTraits>::type
    >::template inner<implicit_conversion_converter<FromTraits,ToTraits>,typename wrap_type<ToTraits>::type>
{
    typedef typename unwrap_value_type<ToTraits>::type       to_type;
    typedef typename unwrap_value_type<FromTraits>::type     from_type;

    typedef typename mpl::list<from_type,reference_,typename unwrap_cv<FromTraits>::type>::type input_traits;

    typedef typename wrap_type<input_traits>::type           input_type;
    typedef typename wrap_type<ToTraits>::type               result_type;

    implicit_conversion_converter() {}
    virtual ~implicit_conversion_converter() {}

    result_type operator()(const valueP& input,valueP& output)
    {
        return result_type(unwrap<input_type>(input, precedence::unwrap)());
    }

    virtual precedence get_precedence()  {return precedence::function;}
    virtual type_detail from_type_info() {return type_id<input_type>();}
};

template<typename FromTraits,typename WrappedTraits,typename ToTraits>
struct implicit_conversion_wrapped_converter 
:   converter_base_selector<
        typename unwrap_indirection<ToTraits>::type,
        typename unwrap_cv<ToTraits>::type
    >::template inner<implicit_conversion_wrapped_converter<FromTraits,WrappedTraits,ToTraits>,typename wrap_type<ToTraits>::type>
{
    typedef typename unwrap_value_type<ToTraits>::type       to_type;
    typedef typename unwrap_value_type<FromTraits>::type     from_type;

    typedef typename mpl::list<from_type,reference_,typename unwrap_cv<FromTraits>::type>::type input_traits;

    typedef typename wrap_type<input_traits>::type           input_type;
    typedef typename wrap_type<ToTraits>::type               result_type;
    typedef typename wrap_type<WrappedTraits>::type          wrapped_type;

    implicit_conversion_wrapped_converter() {}
    virtual ~implicit_conversion_wrapped_converter() {}

    result_type operator()(const valueP& input,valueP& output)
    {
        return wrapped_type(unwrap<input_type>(input, precedence::unwrap)());
    }

    virtual precedence get_precedence()  {return precedence::function;}
    virtual type_detail from_type_info() {return type_id<input_type>();}
};

template<typename FromTraits,typename ToTraits>
struct constructor_converter 
:   converter_base_selector<
        pointer_,
        cv_unqualified
    >::inner<constructor_converter<FromTraits,ToTraits>,typename unwrap_value_type<ToTraits>::type*>
{
    typedef typename unwrap_value_type<ToTraits>::type           to_type;
    typedef typename unwrap_value_type<FromTraits>::type         from_type;

    typedef typename wrap_type<FromTraits>::type                 input_type;
    typedef typename unwrap_value_type<ToTraits>::type*          result_type;
    constructor_converter() {}
    virtual ~constructor_converter() {}
    result_type operator()(const valueP& input,valueP& output)
    {
        result_type result=new to_type(unwrap<input_type>(input, precedence::function)());
        output=wrap_struct<result_type>::wrap(result,input->get_context());
        return result;
    }

    virtual precedence get_precedence()  {return precedence::create;}
    virtual type_detail from_type_info() {return type_id<input_type>();}
};

template<typename FromTraits,typename ToTraits>
struct unwrap_constructor_converter 
:   converter_base_selector<
        typename unwrap_indirection<ToTraits>::type,
        typename unwrap_cv<ToTraits>::type
    >::template inner<unwrap_constructor_converter<FromTraits,ToTraits>,typename wrap_type<ToTraits>::type>

{
    typedef typename unwrap_value_type<ToTraits>::type           to_type;
    typedef typename unwrap_value_type<FromTraits>::type         from_type;

    typedef typename wrap_type<FromTraits>::type*                input_type;
    typedef typename wrap_type<ToTraits>::type                   result_type;
    unwrap_constructor_converter() {}
    virtual ~unwrap_constructor_converter() {}
    result_type operator()(const valueP& input,valueP& output)
    {
        output = wrap_struct<input_type>::wrap(unwrap<input_type>(input)(),input->get_context());
        return unwrap<result_type>(output, precedence::function)();
    }

    virtual precedence get_precedence()  {return precedence::create;}
    virtual type_detail from_type_info() {return type_id<input_type>();}
};

template<typename To>
struct cast_converter
:   converter_base_selector<
        pointer_,
        cv_unqualified
    >::inner<cast_converter<To>,To*>
{
    typedef To           to_type;
    typedef to_type*     result_type;
    cast_converter() {}
    virtual ~cast_converter() {}
    result_type operator()(const valueP& input,valueP& output)
    {
        return static_cast<result_type>(input.get());
    }

    virtual precedence get_precedence() {return precedence::unwrap;}
};

template<typename Fn,typename Wrapper,typename ToTraits>
struct unwrap_converter
:   converter_base_selector<
        typename unwrap_indirection<ToTraits>::type,
        typename unwrap_cv<ToTraits>::type
    >::template inner<unwrap_converter<Fn,Wrapper,ToTraits>,typename wrap_type<ToTraits>::type>        
{
    typedef typename unwrap_value_type<ToTraits>::type       to_type;
    typedef Wrapper                                 wrapper_type;

    typedef typename wrap_type<ToTraits>::type               result_type;

    unwrap_converter(Fn fn) : fn_(fn) {}
    virtual ~unwrap_converter() {}

    result_type operator()(const valueP& input,valueP& output)
    {
        return (static_cast<wrapper_type*>(input.get())->*fn_)();
    }
    virtual precedence get_precedence() {return precedence::unwrap;}
private:
    Fn fn_;
};

template<typename T>
struct intrusive_ptr_conversion_converter 
:   converter_base_selector<
        direct_,
        cv_unqualified
    >::inner<intrusive_ptr_conversion_converter<T>,clipp_ptr<T> >
{
    typedef clipp_ptr<T> to_type;
    typedef T                from_type;
    typedef T*               input_type;
    typedef clipp_ptr<T> result_type;

    intrusive_ptr_conversion_converter() {}
    virtual ~intrusive_ptr_conversion_converter() {}

    result_type operator()(const valueP& input,valueP& output)
    {
        result_type result = unwrap<input_type>(input, precedence::create)();
        output=result;
        return result;
    }

    virtual precedence get_precedence()  {return precedence::unwrap;}
    virtual type_detail from_type_info() {return type_id<input_type>();}
};

}}} //namespace boost::clipp::detail

#endif //BOOST_CLIPP_CONVERTER_HPP_HOLT_03092003
