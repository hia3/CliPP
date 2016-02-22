// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_WRAP_HPP_HOLT_07092003
#define BOOST_CLIPP_WRAP_HPP_HOLT_07092003

#include <boost/clipp/detail/unwrap_type.hpp>
#include <boost/clipp/detail/pre_wrap_value.hpp>
#include <boost/clipp/value.hpp>
#include <boost/clipp/error_messages.hpp>
#include <boost/call_traits.hpp>
#include <boost/clipp/back_reference.hpp>
#include <iostream>

namespace boost { namespace clipp {

namespace detail {

struct wrapper_base;
typedef intrusive_ptr<wrapper_base> wrapperP;

struct wrapper_base : ref_base {
    virtual type_detail from_type_info() = 0;
    virtual wrapperP get_wrapper(decoration d) = 0;
};

template<typename T>
struct wrapper : wrapper_base {
    typedef T input_type;
    virtual valueP operator()(input_type input,context* c) = 0;
};

///Convert between different decorations
template<typename Wrapper,typename T>
struct redecorate_wrapper_base : wrapper<T>
{
    typedef Wrapper wrapper_type;
    typedef typename mpl::list<void,typename unwrap_type<T>::indirection,typename unwrap_type<T>::cv> new_decoration;
    explicit redecorate_wrapper_base(Wrapper* w) : w_(w) {};
    wrapperP get_wrapper(decoration d) {
        if(decoration(unwrap_decoration<new_decoration>::value)==d) return this;
        return w_->get_wrapper(d);
    }
    type_detail from_type_info()    {return w_->from_type_info();}
    wrapper_type& other_wrapper() {
        return *w_;
    }
private:
    intrusive_ptr<wrapper_type> w_;
};

template<typename Wrapper,typename T>
struct address_of_wrapper : redecorate_wrapper_base<Wrapper,T>
{
    typedef typename wrapper<T>::input_type input_type;
    address_of_wrapper(Wrapper* w) : redecorate_wrapper_base<Wrapper,T>(w) {}
    valueP operator()(input_type input,context* c) {
        return this->other_wrapper()(&input,c);
    }
};

template<typename Wrapper,typename T>
struct dereference_wrapper : redecorate_wrapper_base<Wrapper,T>
{
    typedef typename wrapper<T>::input_type input_type;
    dereference_wrapper(Wrapper* w) : redecorate_wrapper_base<Wrapper,T>(w) {}
    valueP operator()(input_type input,context* c) {
        return this->other_wrapper()(*input,c);
    }
};

template<typename Wrapper,typename T>
struct redecorate_wrapper : redecorate_wrapper_base<Wrapper,T>
{
    typedef typename wrapper<T>::input_type input_type;
    explicit redecorate_wrapper(Wrapper* w) : redecorate_wrapper_base<Wrapper,T>(w) {}
    valueP operator()(input_type input,context* c) {
        return this->other_wrapper()(input,c);
    }
};

} 

namespace detail {

//direct
template<typename Indirection,typename CV>
struct wrap_derived {
    template<typename T>
    static intrusive_ptr<detail::wrapper<T> > 
    get_wrapper(T const& input,context* c) 
    {
        typedef detail::wrapper<T> wrapper_type;
        intrusive_ptr<wrapper_type> cvt = static_cast<wrapper_type*>(
            c->get_wrapper(type_id<T>()).get()
        );
        if(cvt) return cvt;
        typedef detail::wrapper<T const&> intermediate_wrapper;
        intrusive_ptr<intermediate_wrapper> icvt = static_cast<intermediate_wrapper*>(
            c->get_wrapper(type_id<T const&>()).get()
        );
        if(!icvt) return nullptr;
        cvt=new redecorate_wrapper<intermediate_wrapper,T>(icvt.get());
        c->add_wrapper(cvt);
        return cvt;
    }
};

template<>
struct wrap_derived<pointer_,cv_unqualified>
{
    template<typename T>
    static intrusive_ptr<detail::wrapper<T*> > 
    get_wrapper(T* input,context* c) 
    {
        typedef detail::wrapper<T*> wrapper_type;
        wrapper_type* wrapper = 
            static_cast<wrapper_type*>(
                c->get_wrapper(type_id<T*>()).get()
            );
        if(wrapper) return wrapper;
        else return static_cast<wrapper_type*>
            (
                c->get_wrapper(type_detail(typeid(*input), decoration::pointer)).get()
            );
    }
};

template<>
struct wrap_derived<pointer_,const_>
{
    template<typename T>
    static intrusive_ptr<detail::wrapper<T const*> > 
    get_wrapper(T const* input,context* c) 
    {
        typedef detail::wrapper<T const*> wrapper_type;
        wrapper_type* wrapper = 
            static_cast<wrapper_type*>(
                c->get_wrapper(type_id<T const*>()).get()
            );
        if(wrapper) return wrapper;
        else return static_cast<wrapper_type*>
            (
                c->get_wrapper(type_detail(typeid(*input), decoration::const_pointer)).get()
            );
    }
};

template<>
struct wrap_derived<reference_,cv_unqualified>
{
    template<typename T>
    static intrusive_ptr<detail::wrapper<T&> > 
    get_wrapper(T& input,context* c) 
    {
        typedef detail::wrapper<T&> wrapper_type;
        wrapper_type* wrapper = 
            static_cast<wrapper_type*>(
                c->get_wrapper(type_id<T&>()).get()
            );
        if(wrapper) return wrapper;
        else return static_cast<wrapper_type*>
            (
                c->get_wrapper(type_detail(typeid(input), decoration::reference)).get()
            );
    }
};

template<>
struct wrap_derived<reference_,const_>
{
    template<typename T>
    static intrusive_ptr<detail::wrapper<T const&> > 
    get_wrapper(T const& input,context* c) 
    {
        typedef detail::wrapper<T const&> wrapper_type;
        wrapper_type* wrapper = 
            static_cast<wrapper_type*>(
                c->get_wrapper(type_id<T const&>()).get()
            );
        if(wrapper) return wrapper;
        else return static_cast<wrapper_type*>
            (
                c->get_wrapper(type_detail(typeid(input),decoration::const_reference)).get()
            );
    }
};

} //namespace detail

namespace detail {

    template<typename T>
    struct wrap_default {
        static valueP wrap(T input,context* c) {
            typedef wrapper<T> wrapper_type;
            intrusive_ptr<wrapper_type> cvt;
            cvt = wrap_derived<BOOST_DEDUCED_TYPENAME unwrap_type<T>::indirection,BOOST_DEDUCED_TYPENAME unwrap_type<T>::cv>::get_wrapper(input,c);
            if(cvt) return (*cvt)(input,c); 
            else throw bad_type_conversion(c,type_id<T>(),typeid(value));
        }
    };

    struct wrap_array {
        template<typename T>
        static valueP wrap(T input[],context* c) {
            return wrap_default<T*>::wrap(input,c);
        }
    };

    struct wrap_clipp_ptr {
        template<typename T>
        static valueP wrap(const T& input,context* c) {
            return wrap_default<BOOST_DEDUCED_TYPENAME T::element_type*>::wrap(input.get(),c);
        }
    };
} //namespace detail

template<typename T>
struct wrap_struct {
    static valueP wrap(T input, context* c) {
        typedef typename mpl::if_<
            is_array<T>,
            detail::wrap_array,
            BOOST_DEDUCED_TYPENAME mpl::if_<
                is_clipp_ptr<T>,
                detail::wrap_clipp_ptr,
                detail::wrap_default<T>
            >::type
        >::type wrap_type;
        return wrap_type::wrap(input,c);
    }
};

template<>
struct wrap_struct<valueP> {
    static valueP wrap(valueP input, context* c) {
        return input;
    }
};

template<>
struct wrap_struct<value*> {
    static valueP wrap(value* input, context* c) {
        if(!input->created()) input->create(c);
        return input;
    }
};

template<>
struct wrap_struct<detail::pre_wrap_value> {
    static valueP wrap(detail::pre_wrap_value input, context* c) {
        return input.get_value(c);
    }
};

template<>
struct wrap_struct<detail::pre_wrap_value const&> {
    static valueP wrap(detail::pre_wrap_value input, context* c) {
        return input.get_value(c);
    }
};

namespace detail {

template<bool pointer,bool arithmetic,bool reference>
struct wrap_traits_chooser {
    template<typename T>
    struct rebind {
        typedef T type;
    };
};

template<>
struct wrap_traits_chooser<false,false,false> {
    template<typename T>
    struct rebind {
        typedef T const& type;
    };
};

}

template <typename T>
struct wrap_traits
{
private:
    typedef detail::wrap_traits_chooser<
         ::boost::is_pointer<T>::value,
         ::boost::is_arithmetic<T>::value,
         ::boost::is_reference<T>::value
      > chooser;
   typedef typename chooser::template rebind<T> bound_type;
public:
   typedef typename bound_type::type type;
};

template<typename T>
inline valueP wrap(T input,context* c)
{
    return wrap_struct<BOOST_DEDUCED_TYPENAME wrap_traits<T>::type>::wrap(input,c);
}

//Since the compiler does not distinguish between reference types and non-reference types properly, we 
//need to do things the hard way
template<typename T>
inline valueP wrap_ref(T const& input1,T const& input2,context* c)
{
    //If input is indeed a reference, it should point to the same location
    if(&input1==&input2) {
        return wrap_struct<T&>::wrap(const_cast<T&>(input1),c);
    }
    else {
        return wrap_struct<T>::wrap(input1,c);
    }
}

template<typename T>
inline valueP wrap_ref(T* input1,T* input2,context* c)
{
    return wrap_struct<T*>::wrap(input1,c);
}

template<typename T>
inline valueP wrap_ref(T& input,context* c)
{
    return wrap_struct<T&>::wrap(input,c);
}

template<typename T>
inline valueP wrap_ref(T* input,context* c)
{
    return wrap_struct<T*>::wrap(input,c);
}

}} //namespace boost::clipp


#endif //BOOST_CLIPP_UNWRAP_HPP_HOLT_03092003
