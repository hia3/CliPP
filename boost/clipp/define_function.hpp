// Copyright Peder Holt 2003-2004. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#pragma once

#include <boost/clipp/config.hpp>
#include <boost/clipp/detail/list.hpp>
#include <boost/clipp/detail/signature_extracter.hpp>
#include <boost/clipp/member_function.hpp>

namespace boost { namespace clipp {

namespace detail {

template<typename C,typename DerivedT>
class define_function {
public:

    //Member function
    template<typename R, typename CT, typename... A>
    member_function<mpl::list<R(CT::*)(A...), R, member_function_tag, cv_unqualified, CT&, A...> >&
    function(const char* name, R(CT::*fn)(A...))
    {
        return derived().define_member(name, fn, boost::type<member_function<mpl::list<R(CT::*)(A...), R, member_function_tag, cv_unqualified, CT&, A...> > >());
    }
    //Const member function
    template<typename R, typename CT, typename... A>
    member_function<mpl::list<R(CT::*)(A...) const, R, member_function_tag, const_, CT const&, A...> >&
    function(const char* name, R(CT::*fn)(A...) const)
    {
        return derived().define_member(name, fn, boost::type<member_function<mpl::list<R(CT::*)(A...) const, R, member_function_tag, const_, CT const&, A...> > >());
    }
    //Free function
    template<typename R, typename... A>
    member_function<mpl::list<R(*)(A...), R, free_function_tag, cv_unqualified, A...> >&
    function(const char* name, R(*fn)(A...))
    {
        return derived().define_member(name, fn, boost::type<member_function<mpl::list<R(*)(A...), R, free_function_tag, cv_unqualified, A...> > >());
    }

    //static Member function
    template<typename R, typename CT, typename... A>
    member_static_function<mpl::list<R(CT::*)(A...), R, member_function_tag, cv_unqualified, CT&, A...> >&
    static_function(const char* name, R(CT::*fn)(A...))
    {
        return derived().define_member(name, fn, boost::type<member_static_function<mpl::list<R(CT::*)(A...), R, member_function_tag, cv_unqualified, CT&, A...> > >());
    }
    //static Const member function
    template<typename R, typename CT, typename... A>
    member_static_function<mpl::list<R(CT::*)(A...) const, R, member_function_tag, const_, CT const&, A...> >&
    static_function(const char* name, R(CT::*fn)(A...) const)
    {
        return derived().define_member(name, fn, boost::type<member_static_function<mpl::list<R(CT::*)(A...) const, R, member_function_tag, const_, CT const&, A...> > >());
    }
    //static Free function
    template<typename R, typename... A>
    member_static_function<mpl::list<R(*)(A...), R, free_function_tag, cv_unqualified, A...> >&
    static_function(const char* name, R(*fn)(A...))
    {
        return derived().define_member(name, fn, boost::type<member_static_function<mpl::list<R(*)(A...), R, free_function_tag, cv_unqualified, A...> > >());
    }

private:
    DerivedT& derived() {return static_cast<DerivedT&>(*this);}
};

} //namespace detail

}} //namespace boost::clipp
