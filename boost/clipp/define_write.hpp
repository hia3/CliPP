// Copyright Peder Holt 2003-2004. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_DEFINE_WRITE_HPP_HOLT_28022004
#define BOOST_CLIPP_DEFINE_WRITE_HPP_HOLT_28022004

#include <boost/clipp/config.hpp>
#include <boost/clipp/detail/list.hpp>
#include <boost/clipp/detail/signature_extracter.hpp>
#include <boost/clipp/detail/cv_category.hpp>
#include <boost/clipp/member_property.hpp>


namespace boost { namespace clipp {

namespace detail {

template<typename C,typename DerivedT>
class define_write {
public:
    //Member variable
    template<typename R,typename CT>
    member_property_write<mpl::list<R CT::*,R,member_data_tag,cv_unqualified,CT&,R> >&
    write(const char* name,R CT::*var) {
        return derived().define_member(name,var,type<member_property_write<mpl::list<R CT::*,R,member_data_tag,cv_unqualified,CT&,R> > >());
    }

    //Member function
    template<typename R,typename CT,typename A0>
    member_property_write<mpl::list<R (CT::*)(A0),R,member_function_tag,cv_unqualified,CT&,A0> >&
    write(const char* name,R (CT::*fn)(A0)) {
        return derived().define_member(name,fn,type<member_property_write<mpl::list<R (CT::*)(A0),R,member_function_tag,cv_unqualified,CT&,A0> > >());
    }
    template<typename R,typename CT,typename A0>
    member_property_write<mpl::list<R (CT::*)(A0) const,R,member_function_tag,const_,CT const&,R,A0> >&
    write(const char* name,R (CT::*fn)(A0) const) {
        return derived().define_member(name,fn,type<member_property_write<mpl::list<R (CT::*)(A0) const,R,member_function_tag,const_,CT const&,A0> > >());
    }

    //Free function
    template<typename R,typename A0>
    static_property_write<mpl::list<R (*)(A0),R,free_function_tag,cv_unqualified,A0> >&
    write(const char* name,R (*fn)(A0)) {
        return derived().define_member(name,fn,type<static_property_write<mpl::list<R (*)(A0),R,free_function_tag,cv_unqualified,A0> > >());
    }
    //Free function as member
    template<typename R,typename CT,typename A0>
    member_property_write<mpl::list<R (*)(CT,A0),R,free_function_tag,cv_unqualified,CT,A0> >&
    write(const char* name,R (*fn)(CT,A0)) {
        return derived().define_member(name,fn,type<member_property_write<mpl::list<R (*)(CT,A0),R,free_function_tag,cv_unqualified,CT,A0> > >());
    }
    //Free reference wrapper
    template<typename T>
    static_property_write<mpl::list<T&,T&,reference_wrapper_tag,cv_unqualified,const T&> >&
    write(const char* name,const reference_wrapper<T>& var) {
        return derived().define_member(name,var,type<static_property_write<mpl::list<T&,T&,reference_wrapper_tag,cv_unqualified,const T&> > >());
    }
private:
    DerivedT& derived() {return static_cast<DerivedT&>(*this);}
};

} //namespace detail

}} //namespace boost::clipp


#endif //BOOST_CLIPP_DEFINE_WRITE_HPP_HOLT_28022004

