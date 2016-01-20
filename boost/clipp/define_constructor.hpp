// Copyright Peder Holt 2003-2004. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#pragma once

#include <boost/clipp/config.hpp>
#include <boost/clipp/detail/list.hpp>
#include <boost/clipp/detail/signature_extracter.hpp>
#include <boost/clipp/member_constructor.hpp>

namespace boost { namespace clipp {

namespace detail {

template<typename C,typename DerivedT,typename StorageP>
class define_constructor {
public:
    //Constructors
    template<typename... A>
    member_constructor<mpl::list<arguments<A...>,C,BOOST_DEDUCED_TYPENAME StorageP::create_tag,cv_unqualified, A...> >&
    constructor(arguments<A...> const& args) 
    {
        return derived().define_member
            (
                "constructor",
                args,
                boost::type<member_constructor<mpl::list<arguments<A...>, C, BOOST_DEDUCED_TYPENAME StorageP::create_tag, cv_unqualified, A...> > >()
            );
    }

    template<typename... A>
    member_constructor<mpl::list<arguments<A...>,C,BOOST_DEDUCED_TYPENAME StorageP::create_tag,cv_unqualified, A...> >&
    constructor(arguments<A...> const& args,value::value_method method) 
    {
        typedef member_constructor<mpl::list<arguments<A...>,C,BOOST_DEDUCED_TYPENAME StorageP::create_tag,cv_unqualified, A...> > constructor_type;
        return derived().define_member("constructor",new constructor_type("constructor",args,method));
    }

    //Free function
    template<typename R, typename... A>
    member_constructor<mpl::list<R(*)(A...), R, free_function_tag, cv_unqualified, A...> >&
    constructor(R(*fn)(A...))
    {
        return derived().define_member("constructor", fn, boost::type<member_constructor<mpl::list<R(*)(A...), R, free_function_tag, cv_unqualified, A...> > >());
    }
    //Free function
    template<typename R, typename... A>
    member_constructor<mpl::list<R(*)(A...), R, free_function_tag, cv_unqualified, A...> >&
        constructor(R(*fn)(A...), value::value_method method)
    {
        typedef member_constructor<mpl::list<R(*)(A...), R, free_function_tag, cv_unqualified, A...> > constructor_type;
        return derived().define_member("constructor", new constructor_type("constructor", fn, method));
    }

private:
    DerivedT& derived() {return static_cast<DerivedT&>(*this);}
};

} //namespace detail

}} //namespace boost::clipp
