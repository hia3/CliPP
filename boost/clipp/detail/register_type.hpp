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

}}} //namespace boost::clipp::detail

#endif //BOOST_CLIPP_DETAIL_REGISTER_TYPE_HPP_HOLT_20030614
