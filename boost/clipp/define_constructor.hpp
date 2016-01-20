#if !defined (BOOST_PP_IS_ITERATING)

// Copyright Peder Holt 2003-2004. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_DEFINE_CONSTRUCTOR_HPP_HOLT_29022004
#define BOOST_CLIPP_DEFINE_CONSTRUCTOR_HPP_HOLT_29022004

#include <boost/clipp/config.hpp>
#include <boost/clipp/detail/list.hpp>
#include <boost/clipp/detail/signature_extracter.hpp>
#include <boost/clipp/member_constructor.hpp>

#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing.hpp>

namespace boost { namespace clipp {

namespace detail {

template<typename C,typename DerivedT,typename StorageP>
class define_constructor {
public:
    //Constructors
    template<BOOST_PP_ENUM_PARAMS(BOOST_CLIPP_MAX_ARITY, typename A)>
    member_constructor<mpl::list<arguments<BOOST_PP_ENUM_PARAMS(BOOST_CLIPP_MAX_ARITY, A)>,C,BOOST_DEDUCED_TYPENAME StorageP::create_tag,cv_unqualified BOOST_PP_ENUM_TRAILING_PARAMS(BOOST_CLIPP_MAX_ARITY, A)> >&
    constructor(arguments<BOOST_PP_ENUM_PARAMS(BOOST_CLIPP_MAX_ARITY, A)> const& args) 
    {
        return derived().define_member("constructor",args,boost::type<member_constructor<mpl::list<arguments<BOOST_PP_ENUM_PARAMS(BOOST_CLIPP_MAX_ARITY, A)>,C,BOOST_DEDUCED_TYPENAME StorageP::create_tag,cv_unqualified BOOST_PP_ENUM_TRAILING_PARAMS(BOOST_CLIPP_MAX_ARITY, A)> > >());
    }

    template<BOOST_PP_ENUM_PARAMS(BOOST_CLIPP_MAX_ARITY, typename A)>
    member_constructor<mpl::list<arguments<BOOST_PP_ENUM_PARAMS(BOOST_CLIPP_MAX_ARITY, A)>,C,BOOST_DEDUCED_TYPENAME StorageP::create_tag,cv_unqualified BOOST_PP_ENUM_TRAILING_PARAMS(BOOST_CLIPP_MAX_ARITY, A)> >&
    constructor(arguments<BOOST_PP_ENUM_PARAMS(BOOST_CLIPP_MAX_ARITY, A)> const& args,value::value_method method) 
    {
        typedef member_constructor<mpl::list<arguments<BOOST_PP_ENUM_PARAMS(BOOST_CLIPP_MAX_ARITY, A)>,C,BOOST_DEDUCED_TYPENAME StorageP::create_tag,cv_unqualified BOOST_PP_ENUM_TRAILING_PARAMS(BOOST_CLIPP_MAX_ARITY, A)> > constructor_type;
        return derived().define_member("constructor",new constructor_type("constructor",args,method));
    }

#define BOOST_PP_ITERATION_PARAMS_1 (3, (0, BOOST_CLIPP_MAX_ARITY, "boost/clipp/define_constructor.hpp"))
#include BOOST_PP_ITERATE()
private:
    DerivedT& derived() {return static_cast<DerivedT&>(*this);}
};

} //namespace detail

}} //namespace boost::clipp

#endif //BOOST_CLIPP_DEFINE_CONSTRUCTOR_HPP_HOLT_29022004

#else //BOOST_PP_IS_ITERATING

#define N BOOST_PP_ITERATION()
    //Free function
    template<typename R BOOST_PP_ENUM_TRAILING_PARAMS(N, typename A)>
    member_constructor<mpl::list<R (*)(BOOST_PP_ENUM_PARAMS(N, A)),R,free_function_tag,cv_unqualified BOOST_PP_ENUM_TRAILING_PARAMS(N, A)> >&
    constructor(R (*fn)(BOOST_PP_ENUM_PARAMS(N, A))) 
    {
        return derived().define_member("constructor",fn,boost::type<member_constructor<mpl::list<R (*)(BOOST_PP_ENUM_PARAMS(N, A)),R,free_function_tag,cv_unqualified BOOST_PP_ENUM_TRAILING_PARAMS(N, A)> > >());
    }
    //Free function
    template<typename R BOOST_PP_ENUM_TRAILING_PARAMS(N, typename A)>
    member_constructor<mpl::list<R (*)(BOOST_PP_ENUM_PARAMS(N, A)),R,free_function_tag,cv_unqualified BOOST_PP_ENUM_TRAILING_PARAMS(N, A)> >&
    constructor(R (*fn)(BOOST_PP_ENUM_PARAMS(N, A)),value::value_method method) 
    {
        typedef member_constructor<mpl::list<R (*)(BOOST_PP_ENUM_PARAMS(N, A)),R,free_function_tag,cv_unqualified BOOST_PP_ENUM_TRAILING_PARAMS(N, A)> > constructor_type;
        return derived().define_member("constructor",new constructor_type("constructor",fn,method));
    }
#undef N

#endif //BOOST_PP_IS_ITERATING
/*
    template<BOOST_PP_ENUM_TRAILING_PARAMS(N, typename A)>
    member_constuctor<mpl::list<constructor<BOOST_PP_ENUM_PARAMS(N, A)>,C*,constructor_tag,cv_unqualified,BOOST_PP_ENUM_TRAILING_PARAMS(N, A)> >&
    operator[](constructor<BOOST_PP_ENUM_PARAMS(N, A)> const& args) 
    {
        return derived().define_member(name,fn,boost::type<member_constuctor<mpl::list<constructor<BOOST_PP_ENUM_PARAMS(N, A)>,C*,constructor_tag,cv_unqualified,BOOST_PP_ENUM_TRAILING_PARAMS(N, A)> > >());
    }
*/

