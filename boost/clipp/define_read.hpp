// Copyright Peder Holt 2003-2004. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_DEFINE_READ_HPP_HOLT_28022004
#define BOOST_CLIPP_DEFINE_READ_HPP_HOLT_28022004

#include <boost/clipp/config.hpp>
#include <boost/clipp/detail/list.hpp>
#include <boost/clipp/detail/signature_extracter.hpp>
#include <boost/clipp/detail/cv_category.hpp>
#include <boost/clipp/member_property.hpp>

namespace boost { namespace clipp {

namespace detail {

template<typename C,typename DerivedT>
class define_read {
public:
    template<typename Fn>
    member& read(const char* name,Fn fn) {
        return read_selector(name,fn,detail::get_data_tag(fn));
    };
private:
    template<typename T>
    member& read_selector(const char* name,T fn,free_data_tag) {
        typedef mpl::list<T,T,free_data_tag,cv_unqualified> signature_type;
        return derived().define_member(name,fn,boost::type<static_property_read<signature_type> >());
    }
    //Member variable
    template<typename R,typename CT>
    member_property_read<mpl::list<R CT::*,R,member_data_tag,cv_unqualified,CT&> >&
    read_selector(const char* name,R CT::*var,member_data_tag) {
        return derived().define_member(name,var,type<member_property_read<mpl::list<R CT::*,R,member_data_tag,cv_unqualified,CT&> > >());
    }

    //Member function
    template<typename R,typename CT>
    member_property_read<mpl::list<R (CT::*)(),R,member_function_tag,cv_unqualified,CT&> >&
    read_selector(const char* name,R (CT::*fn)(),member_function_tag) {
        return derived().define_member(name,fn,type<member_property_read<mpl::list<R (CT::*)(),R,member_function_tag,cv_unqualified,CT&> > >());
    }
    template<typename R,typename CT>
    member_property_read<mpl::list<R (CT::*)() const,R,member_function_tag,const_,CT const&> >&
    read_selector(const char* name,R (CT::*fn)() const,member_function_tag) {
        return derived().define_member(name,fn,type<member_property_read<mpl::list<R (CT::*)() const,R,member_function_tag,const_,CT const&> > >());
    }

    //Free function
    template<typename R>
    static_property_read<mpl::list<R (*)(),R,free_function_tag,cv_unqualified> >&
    read_selector(const char* name,R (*fn)(),free_function_tag) {
        return derived().define_member(name,fn,type<static_property_read<mpl::list<R (*)(),R,free_function_tag,cv_unqualified> > >());
    }
    //Free function as member
    template<typename R,typename CT>
    member_property_read<mpl::list<R (*)(CT),R,free_function_tag,cv_unqualified,CT> >&
    read_selector(const char* name,R (*fn)(CT),free_function_tag) {
        return derived().define_member(name,fn,type<member_property_read<mpl::list<R (*)(CT),R,free_function_tag,cv_unqualified,CT> > >());
    }
    //Free reference wrapper
    template<typename T>
    static_property_read<mpl::list<T&,T&,reference_wrapper_tag,cv_unqualified> >&
    read_selector(const char* name,const reference_wrapper<T>& var,reference_wrapper_tag) {
        return derived().define_member(name,var,type<static_property_read<mpl::list<T&,T&,reference_wrapper_tag,cv_unqualified> > >());
    }
private:
    DerivedT& derived() {return static_cast<DerivedT&>(*this);}
};

} //namespace detail

}} //namespace boost::clipp


#endif //BOOST_CLIPP_DEFINE_READ_HPP_HOLT_28022004

