// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_REDECORATE_WRAPPER_HPP_HOLT_14092003
#define BOOST_CLIPP_REDECORATE_WRAPPER_HPP_HOLT_14092003

#include <boost/clipp/wrap.hpp>
#include <boost/clipp/ref_base.hpp>
#include <boost/clipp/decoration.hpp>

namespace boost { namespace clipp { namespace detail {

/*
T:
    all
T const: 
    all
T&
    T&
    T*
T const&
    all
T*
    T&
    T*
T const*
    T&
    T*
    T const&
    T const*
*/

template<typename indirection,typename cv>
struct wrapper_base_selector;

template<typename Wrapper,typename T>
struct decorated_wrapper_base : wrapper<T> 
{
    typedef Wrapper           wrapper_type;
    wrapper_type& derived() {
        return static_cast<wrapper_type&>(*this);
    }
};

template<>
struct wrapper_base_selector<direct_,cv_unqualified> 
{
    template<typename Wrapper,typename T>
    struct inner : decorated_wrapper_base<Wrapper,T> {
        wrapperP get_wrapper(decoration d) {
            typedef Wrapper           wrapper_type;
            typedef typename wrapper_type::from_type  from_type;
            switch(d) {
            case decoration::direct:
                return this;
            case decoration::const_direct:
                return new redecorate_wrapper<wrapper_type,from_type const>(&this->derived());
            case decoration::reference:
                return new redecorate_wrapper<wrapper_type,from_type&>(&this->derived());
            case decoration::const_reference:
                return new redecorate_wrapper<wrapper_type,from_type const&>(&this->derived());
            case decoration::pointer:
                return new dereference_wrapper<wrapper_type,from_type*>(&this->derived());
            case decoration::const_pointer:
                return new dereference_wrapper<wrapper_type,from_type const*>(&this->derived());
            default:
                return nullptr;
            }
        }
    };
};

template<>
struct wrapper_base_selector<direct_,const_> 
{
    template<typename Wrapper,typename T>
    struct inner : decorated_wrapper_base<Wrapper,T> {
        wrapperP get_wrapper(decoration d) {
            typedef Wrapper           wrapper_type;
            typedef typename wrapper_type::from_type  from_type;
            switch(d) {
            case decoration::direct:
                return new redecorate_wrapper<wrapper_type,from_type>(&this->derived());
            case decoration::const_direct:
                return this;
            case decoration::reference:
                return new redecorate_wrapper<wrapper_type,from_type&>(&this->derived());
            case decoration::const_reference:
                return new redecorate_wrapper<wrapper_type,from_type const&>(&this->derived());
            case decoration::pointer:
                return new dereference_wrapper<wrapper_type,from_type*>(&this->derived());
            case decoration::const_pointer:
                return new dereference_wrapper<wrapper_type,from_type const*>(&this->derived());
            default:
                return nullptr;
            }
        }
    };
};


template<>
struct wrapper_base_selector<pointer_,cv_unqualified> 
{
    template<typename Wrapper,typename T>
    struct inner : decorated_wrapper_base<Wrapper,T> {
        wrapperP get_wrapper(decoration d) {
            typedef Wrapper           wrapper_type;
            typedef typename wrapper_type::from_type  from_type;
            switch(d) {
            case decoration::pointer:
                return this;
            case decoration::reference:
                return new address_of_wrapper<wrapper_type,from_type&>(&this->derived());
            default:
                return nullptr;
            }
        }
    };
};

template<>
struct wrapper_base_selector<pointer_,const_> 
{
    template<typename Wrapper,typename T>
    struct inner : decorated_wrapper_base<Wrapper,T> {
        wrapperP get_wrapper(decoration d) {
            typedef Wrapper           wrapper_type;
            typedef typename wrapper_type::from_type  from_type;
            switch(d) {
            case decoration::pointer:
                return new redecorate_wrapper<wrapper_type,from_type*>(&this->derived());
            case decoration::const_pointer:
                return this;
            case decoration::reference:
                return new address_of_wrapper<wrapper_type,from_type&>(&this->derived());
            case decoration::const_reference:
                return new address_of_wrapper<wrapper_type,from_type const&>(&this->derived());
            default:
                return nullptr;
            }
        }
    };
};

template<>
struct wrapper_base_selector<reference_,cv_unqualified> 
{
    template<typename Wrapper,typename T>
    struct inner : decorated_wrapper_base<Wrapper,T> {
        wrapperP get_wrapper(decoration d) {
            typedef Wrapper           wrapper_type;
            typedef typename wrapper_type::from_type  from_type;
            switch(d) {
            case d_pointer:
                return new dereference_wrapper<wrapper_type,from_type*>(&this->derived());
            case d_reference:
                return this;
            default:
                return nullptr;
            }
        }
    };
};

template<>
struct wrapper_base_selector<reference_,const_> 
{
    template<typename Wrapper,typename T>
    struct inner : decorated_wrapper_base<Wrapper,T> {
        wrapperP get_wrapper(decoration d) {
            typedef Wrapper           wrapper_type;
            typedef typename wrapper_type::from_type  from_type;
            switch(d) {
/*            case decoration::direct:
                return new redecorate_wrapper<wrapper_type,from_type>(&this->derived());
            case decoration::const_direct:
                return new redecorate_wrapper<wrapper_type,from_type const>(&this->derived());*/
            case decoration::pointer:
                return new dereference_wrapper<wrapper_type,from_type*>(&this->derived());
            case decoration::const_pointer:
                return new dereference_wrapper<wrapper_type,from_type const*>(&this->derived());
            case decoration::reference:
                return new redecorate_wrapper<wrapper_type,from_type&>(&this->derived());
            case decoration::const_reference:
                return this;            
            default:
                return nullptr;
            }
        }
    };
};

template<typename FromTraits,typename ToTraits>
struct constructor_wrapper 
:   wrapper_base_selector<
        typename unwrap_indirection<FromTraits>::type,
        typename unwrap_cv<FromTraits>::type
    >::template inner<constructor_wrapper<FromTraits,ToTraits>,typename wrap_type<FromTraits>::type>

{
    typedef typename unwrap_value_type<ToTraits>::type           to_type;
    typedef typename unwrap_value_type<FromTraits>::type         from_type;

    typedef typename wrap_type<FromTraits>::type                 input_type;
    constructor_wrapper() {}
    valueP operator()(input_type input,context* c)
    {
        to_type* result = new to_type(input);
        result->create(c);
        return result;
    }
    virtual type_detail from_type_info()    {return type_id<input_type>();}
};

template<typename From>
struct cast_wrapper : wrapper_base_selector<
        pointer_,
        cv_unqualified
    >::inner<cast_wrapper<From>,From*>
{
    typedef From             from_type;
    typedef from_type*       input_type;
    cast_wrapper() {}
    valueP operator()(input_type input,context* c)
    {
        if(input && !input->created()) input->create(c);
        return input;
    }
    virtual type_detail from_type_info()    {return type_id<input_type>();}
};


}}} //namespace boost::clipp::detail

#endif //BOOST_CLIPP_REDECORATE_WRAPPER_HPP_HOLT_14092003
