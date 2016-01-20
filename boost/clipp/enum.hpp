// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_ENUM_HPP_HOLT_04122003
#define BOOST_CLIPP_ENUM_HPP_HOLT_04122003

#include "boost/clipp/class.hpp"
#include "boost/clipp/enum_wrapper.hpp"

namespace boost { namespace clipp {

template<class EnumT>
class enum_ : private class_<enum_wrapper<EnumT>,object,EnumT>
{
public:
    //Construct a new class, or retrieve old class definition
    enum_(const std::string& name,context* c)
    : class_<enum_wrapper<EnumT>,object,EnumT>(name,c)
    {}

    static_property_read<mpl::list<EnumT,EnumT,detail::free_data_tag,detail::cv_unqualified> >&
    value(const char* name,EnumT var) {
        return define_member(name,var,boost::type<static_property_read<mpl::list<EnumT,EnumT,detail::free_data_tag,detail::cv_unqualified> > >());
    }

    static_property_read<mpl::list<EnumT,EnumT,detail::free_data_tag,detail::cv_unqualified> >&
    global_value(const char* name,EnumT var) {
        typedef static_property_read<mpl::list<EnumT,EnumT,detail::free_data_tag,detail::cv_unqualified> > property_t;
        property_t& prop = define_member(name,var,boost::type<property_t>());
        get_context()->global()->insert(name,&prop);
        return prop;
    }
    valueP global_value(const std::string& name,EnumT input)
    {
        valueP output = value(name,input);
        get_context()->global()->insert(name,output);
    }
};

}} // namespace boost::clipp

#endif //BOOST_CLIPP_ENUM_HPP_HOLT_04122003
