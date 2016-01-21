// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_UNWRAP_HPP_HOLT_03092003
#define BOOST_CLIPP_UNWRAP_HPP_HOLT_03092003

#include <boost/clipp/detail/unwrap_type.hpp>
#include <boost/clipp/value.hpp>
#include <boost/clipp/back_reference.hpp>
#include <boost/clipp/error_messages.hpp>

namespace boost { namespace clipp {

namespace detail {

struct converter_base;
typedef intrusive_ptr<converter_base> converterP;

struct converter_base : ref_base {
    virtual type_detail to_type_info() = 0;
    virtual type_detail from_type_info() {return typeid(value);}
    virtual converterP get_converter(decoration d) = 0;
    virtual precedence get_precedence() = 0;
};

template<typename T>
struct converter : converter_base {
    typedef T result_type;
    virtual result_type operator()(const valueP& input,valueP& output) = 0;
    virtual type_detail to_type_info()      {return type_id<result_type>();}
};

} //namespace detail

template<typename T>
struct unwrap;

namespace detail {

template<typename T>
struct is_unwrap_helper {
    static type_traits::no_type BOOST_TT_DECL _m_check(...);
    template<class U>
    static type_traits::yes_type BOOST_TT_DECL _m_check(unwrap<U> (*)());

    typedef T (*fn)();                                                          

    BOOST_STATIC_CONSTANT(bool, value = 
        sizeof(_m_check(((fn)0)) ) == sizeof(type_traits::yes_type)
    );
};

//Forward define this converter for GCC
template<typename T>
struct intrusive_ptr_conversion_converter;

}//namespace detail


template<typename T>
struct unwrap 
{
    typedef T result_type;
    typedef typename result_type_selector<result_type>::type argument_type;
    typedef detail::converter<argument_type> converter_type;
    unwrap(const unwrap& rhs) :
        wrapped_(rhs.wrapped_),
        local_(rhs.local_),
        precedence_(rhs.precedence_),
        converter_(rhs.converter_)
    {}
    unwrap(const valueP& wrapped,precedence p = precedence::create) : wrapped_(wrapped) , precedence_(p) {}
    void set(const valueP& wrapped) {
        wrapped_=wrapped;
        converter_ = nullptr;
    }
    result_type operator()() const {
        return internal_unwrap(is_back_reference<result_type>());
    }    
    bool ok() const {
        return install_converter();
    }
    valueP input() const {
        return wrapped_;
    }
    context* get_context() const {
        return wrapped_->get_context();
    }
private:
    result_type internal_unwrap(mpl::bool_<true>) const {
        if(!install_converter()) {
            if(!wrapped_) throw bad_type_conversion("null",type_id<argument_type>().name());
            else          throw bad_type_conversion(wrapped_->get_context(),wrapped_->type(),type_id<argument_type>());
        }
        return result_type(wrapped_,(*converter_)(wrapped_,local_));
    }
    result_type internal_unwrap(mpl::bool_<false>) const {
        if(!install_converter()) {
            if(!wrapped_) throw bad_type_conversion("null",type_id<argument_type>().name());
            else          throw bad_type_conversion(wrapped_->get_context(),wrapped_->type(),type_id<argument_type>());
        }
        return (*converter_)(wrapped_,local_);
    }

    bool install_converter() const
    {
        if(converter_) return true;
        if(!wrapped_) return false;
        return install_converter_internal(BOOST_CLIPP_UNWRAP_TYPE(argument_type));
    }

    template<typename Traits>
    bool install_converter_internal(Traits t) const{
        typedef typename detail::unwrap_value_type<Traits>::type actual_type;
        return install_converter_internal_2(t,is_clipp_ptr<actual_type>());
    }
    template<typename Traits>
    bool install_converter_internal_2(Traits t,mpl::false_) const{
        converter_ = static_cast<converter_type*>(
                        wrapped_->get_converter_from_this(type_id_traits(t),precedence_,wrapped_).get()
                     );
        return converter_ != nullptr;
    }
    template<typename Traits>
    bool install_converter_internal_2(Traits t,mpl::true_) const{
        typedef typename detail::unwrap_value_type<Traits>::type actual_type;
        typedef typename actual_type::element_type element_type;
        typedef detail::intrusive_ptr_conversion_converter<element_type> element_converter;
        intrusive_ptr<element_converter> cvt = new element_converter();
        detail::converterP decvt = cvt->get_converter(decoration(detail::unwrap_decoration<Traits>::value));
        converter_ = static_cast<converter_type*>(decvt.get());
        return converter_ != nullptr;
    }
    mutable valueP wrapped_;
    mutable precedence precedence_;
    mutable intrusive_ptr<converter_type> converter_;
    mutable valueP local_;
};


}} //namespace boost::clipp

/*
template<typename T>
struct unwrap_value_base
{
    unwrap_value_base(valueP wrapped,precedence p) 
    :   wrapped_(wrapped) 
    ,   precedence_(precedence)
    ,   converter_(nullptr) {}
protected:
    bool install_converter() 
    {
        if(converter_) return true;
        if(!wrapped_) return false;
        return install_converter_internal(BOOST_CLIPP_UNWRAP_TYPE(T));
    }
    valueP wrapped_;
    converterP converter_;
    precedence precedence_;
private:
    template<typename Traits>
    bool install_converter_internal(Traits) {
        typedef unwrap_cv<Traits>::type type_cv;
        typedef unwrap_indirection<Traits>::type type_indirection;
        value::cv_enum cv=value::cv_enum(unwrap_cv_enum<Traits>::value);
        value::indirection_enum indirection=value::indirection_enum(unwrap_indirection_enum<Traits>::value);
        converter_ = wrapped_->get_converter_from_this(
                        typeid(unwrap_value_type<Traits>::type),
                        decoration(unwrap_decoration<Traits>::value)),
                        precedence_
                     );
        );
        return converter_! = nullptr;
    }
};

template<typename T>
struct unwrap_value_direct : unwrap_value_base<T>
{
    typedef unwrap_value_base<T> base;
    unwrap_value_direct(valueP wrapped,precedence p) : base(wrapped,p) {}
    union result {
        void* input;
        T output;
    };
    T operator()() {
        if(!install_converter()) throw exception();
        result r;
        r.input=converter_->convert(wrapped_,wrapped_);
        return r.output;
    }
};

template<typename T>
struct unwrap_value_indirect : unwrap_value_base<T> {
    unwrap_value_indirect(valueP wrapped,precedence p) : unwrap_value_base<T>(wrapped,p) {}
    T operator()() {
        if(!install_converter()) throw exception();
        return reinterpret_cast<T>(converter_->convert(wrapped_,local_));
    }
protected:
    intrusive_ptr<value> local_;
};

template<typename T>
struct unwrap_value_reference : unwrap_value_base<T> {
    unwrap_value_reference(valueP wrapped,precedence p) : unwrap_value_base<T>(wrapped,p) {}
    T operator()() {
        if(!install_converter()) throw exception();
        return convert_pointer_to_reference(BOOST_CLIPP_UNWRAP_TYPE(T));
    }
protected:
    intrusive_ptr<value> local_;
private:
    template<typename Traits>
    T convert_pointer_to_reference(Traits) {
        typedef mpl::list<extract_type<Traits>::type,pointer_,extract_cv<Traits>::type> PointerTraits;
        typedef wrap_type<PointerTraits>::type pointer_type;
        pointer_type result =  reinterpret_cast<pointer_type>(converter_->convert(wrapped_,local_));
        return *result;
    }
};

template<typename T>
struct unwrap_selector {
    BOOST_STATIC_CONSTANT(bool,direct_possible=(unwrap_type<T>::size <= sizeof(void*)));
    typedef typename mpl::if_<
        is_same<unwrap_type<T>::indirection,direct_>,
        mpl::if_<
            mpl::bool_<direct_possible>,
            unwrap_value_direct<T>,
            unwrap_value_reference<T const&>
        >::type,
        mpl::if_<
            is_same<unwrap_type<T>::indirection,reference_>,
            unwrap_value_reference<T>,          
            unwrap_value_indirect<T>
        >::type
    >::type type;
};

} //namespace detail

template<typename T>
struct unwrap : detail::unwrap_selector<T>::type
{
    typedef detail::unwrap_selector<T>::type base;
    unwrap(valueP wrapped,precedence p=precedence::create) : base(wrapped,p) {}
};
*/
#endif //BOOST_CLIPP_UNWRAP_HPP_HOLT_03092003
