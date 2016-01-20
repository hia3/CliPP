// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_DETAIL_REGISTER_TYPE_HPP_HOLT_20030614
#define BOOST_CLIPP_DETAIL_REGISTER_TYPE_HPP_HOLT_20030614

#include <boost/clipp/is_container.hpp>
#include <boost/clipp/container_wrapper.hpp>

namespace boost { namespace clipp { namespace detail {

struct register_value_class
{
    typedef bool result_type;
    template <class T>
    static result_type do_register(boost::type<T>,context* c)
    {
        T::init(c);
        return true;
    }
};

struct register_report_error
{
    typedef bool result_type;
    template<class T>
    static result_type do_register(boost::type<T>,context* c)
    {
        return false;
    }
};

struct register_container
{
    typedef bool result_type;
    template <class T>
    static result_type do_register(boost::type<T>,context* c)
    {
        container_wrapper<T>::init(c);
        return true;
    }
};

struct register_back_reference
{
    typedef bool result_type;
    template <class T>
    static result_type do_register(boost::type<T>,context* c)
    {
        typedef typename T::reference_type value_type;
        c->add_synonym(type_id<T>(),type_id<value_type>());
        return register_traits(BOOST_CLIPP_UNWRAP_TYPE(value_type),c);
    }
};

template<typename T>
bool register_type(boost::type<T> t,context* c)
{
    if(c->get_prototype(typeid(T))) return true;
    typedef typename mpl::if_<
        is_class<T>, 
        BOOST_DEDUCED_TYPENAME mpl::if_<
            is_base_and_derived<value,T>, 
            register_value_class,
            BOOST_DEDUCED_TYPENAME mpl::if_<
                is_container<T>,
                register_container,
                BOOST_DEDUCED_TYPENAME mpl::if_<
                    is_back_reference<T>,
                    register_back_reference,
                    register_report_error
                >::type
            >::type
        >::type, 
        register_report_error
    >::type register_struct;
    return register_struct::do_register(t,c);
}

template<typename Traits>
bool register_traits(Traits ,context* c)
{
    return register_type(boost::type<BOOST_DEDUCED_TYPENAME unwrap_value_type<Traits>::type>(),c);
}
/*
template<typename T>
struct register_container
{
    static bool 
    typedef bool result_type;
    template <class T>
    static result_type execute(T* p,context* c)
    {
        typedef if_<
                    is_set<T>,
                    set_wrapper<T>,
                    if_<
                        is_vector<T>,
                        vector_wrapper<T>,
                        if_<is_list<T>,
                            list_wrapper<T>,
                            if_<
                                is_valarray<T>,
                                valarray_wrapper<T>
                            >,
                            invalid_wrapper
                        >
                    >
                    >::type type;
        return type::init(c);
    }
};

struct register_value_class
{
    typedef bool result_type;
    template <class T>
    static result_type execute(T* p,context* c)
    {
        return T::init(c);
    }
};

struct register_float
{
    typedef bool result_type;
    template<class T>
    static result_type execute(T* p,context* c)
    {
        return float_wrapper<T>::init(c);
    }
};

struct register_integral
{
    typedef bool result_type;
    template<class T>
    static result_type execute(T* p,context* c)
    {
        return integral_wrapper<T>::init(currentCliManager()->ip());
    }
    static result_type execute(bool* p,context* c)
    {
        //return boolean_wrapper::init(c);
    }
};

struct register_report_error
{
    typedef bool result_type;
    template<class T>
    static result_type execute(T* p)
    {
        return false;
    }
};

struct register_type : dynamic_generator
{
    register_type(context* c) : context_(c) {}
    typedef bool result_type;
    template <class T>
    result_type execute(T* p)
    {
        return false;
/*        if(context_->isRegistered(typeid(T))) return true;
        typedef typename mpl::if_<
            is_container<T>, 
            register_container, 
            typename mpl::if_<
                is_class<T>, 
                typename mpl::if_<
                    is_base_and_derived<value,T>, 
                    register_value_class, 
                    register_report_error
                >::type, 
                typename mpl::if_<
                    is_float<T>,
                    register_float,
                    typename mpl::if_<
                        is_integral<T>,
                        register_integral,
                        register_report_error
                    >::type
                >::type
            >::type
        >::type type;
        return type::execute(p,context_);
    }
    context* context_;
};*/

}}} //namespace boost::clipp::detail

#endif //BOOST_CLIPP_DETAIL_REGISTER_TYPE_HPP_HOLT_20030614
