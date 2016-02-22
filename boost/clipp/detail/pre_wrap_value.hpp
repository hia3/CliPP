// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_PRE_WRAP_VALUE_HPP_HOLT_18102003
# define BOOST_CLIPP_PRE_WRAP_VALUE_HPP_HOLT_18102003

#include <boost/clipp/value.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/type_traits/is_base_and_derived.hpp>

namespace boost { namespace clipp { 

//Forward declare wrap_struct for GCC
template<typename T>
struct wrap_struct;

namespace detail { 

struct pre_wrap_value
{
    struct value_wrapper_internal_base {
        virtual ~value_wrapper_internal_base() {}
        virtual valueP get_value(context* c) = 0;
    };
    typedef boost::shared_ptr<value_wrapper_internal_base> wrapperP;

    pre_wrap_value() {}
    pre_wrap_value(const pre_wrap_value& rhs) 
    :   base_(rhs.base_)
    ,   value_(rhs.value_)
    {}
    pre_wrap_value& operator=(const pre_wrap_value& rhs) {
        if(this!=&rhs) {
            value_=rhs.value_;
            base_=rhs.base_;
        }
        return *this;
    }

    template<typename T>
    const pre_wrap_value& set(T t) {
        install_wrapper(t,BOOST_CLIPP_UNWRAP_TYPE(T));
        return *this;
    }

    bool empty() {return !value_ && !base_;}
    valueP get_value(context* c) {
        if(base_) value_=base_->get_value(c);
        if(value_) base_.reset();
        if(value_ && !value_->created()) value_->create(c);
        return value_;
    }

    //Internal value wrapper
    template<typename T>
    struct value_wrapper_internal : value_wrapper_internal_base
    {
        value_wrapper_internal(T t) :t_(t) {}
        virtual ~value_wrapper_internal() {}
        valueP get_value(context* c) {
            return wrap_struct<T>::wrap(t_,c);
        }
    private:
        T t_;
    };

    struct install_value_helper {
        template<typename T>
        static void install(pre_wrap_value* wrapper,T t) {
            wrapper->value_=t;
        }
    };
    template<typename T>
    struct install_wrapper_helper {
        static void install(pre_wrap_value* wrapper,T t) {
            wrapper->base_=wrapperP(new value_wrapper_internal<T>(t));
        }        
    };

    template<typename T,typename Traits>
    void install_wrapper(T t,Traits)  {
        typedef typename mpl::if_<
            BOOST_DEDUCED_TYPENAME mpl::and_<
                is_base_and_derived<
                    value,
                    BOOST_DEDUCED_TYPENAME unwrap_value_type<Traits>::type
                >,
                is_same<pointer_,BOOST_DEDUCED_TYPENAME unwrap_indirection<Traits>::type>
            >::type,
            install_value_helper,
            install_wrapper_helper<T>
        >::type helper;
        helper::install(this,t);
    }
    wrapperP base_;
    valueP value_;
};

}}} // namespace boost::clipp::detail

#endif // BOOST_CLIPP_PRE_WRAP_VALUE_HPP_HOLT_18102003
