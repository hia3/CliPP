#if !defined (BOOST_PP_IS_ITERATING)

// Copyright Peder Holt 2003-2004. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_DEFINE_FUNCTION_HPP_HOLT_29022004
#define BOOST_CLIPP_DEFINE_FUNCTION_HPP_HOLT_29022004

#include <boost/clipp/config.hpp>
#include <boost/clipp/detail/list.hpp>
#include <boost/clipp/detail/signature_extracter.hpp>
#include <boost/clipp/member_function.hpp>

#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing.hpp>

namespace boost { namespace clipp {

namespace detail {

template<typename C,typename DerivedT>
class define_function {
public:
#define BOOST_PP_ITERATION_PARAMS_1 (3, (0, BOOST_CLIPP_MAX_ARITY, "boost/clipp/define_function.hpp"))
#include BOOST_PP_ITERATE()
private:
    DerivedT& derived() {return static_cast<DerivedT&>(*this);}
};

} //namespace detail

}} //namespace boost::clipp

#endif //BOOST_CLIPP_DEFINE_READ_HPP_HOLT_28022004

#else //BOOST_PP_IS_ITERATING

#define N BOOST_PP_ITERATION()

    //Member function
    template<typename R,typename CT BOOST_PP_ENUM_TRAILING_PARAMS(N, typename A)>
    member_function<mpl::list<R (CT::*)(BOOST_PP_ENUM_PARAMS(N, A)),R,member_function_tag,cv_unqualified,CT& BOOST_PP_ENUM_TRAILING_PARAMS(N, A)> >&
    function(const char* name,R (CT::*fn)(BOOST_PP_ENUM_PARAMS(N, A))) 
    {
        return derived().define_member(name,fn,boost::type<member_function<mpl::list<R (CT::*)(BOOST_PP_ENUM_PARAMS(N, A)),R,member_function_tag,cv_unqualified,CT& BOOST_PP_ENUM_TRAILING_PARAMS(N, A)> > >());
    }

    //Const member function
    template<typename R,typename CT BOOST_PP_ENUM_TRAILING_PARAMS(N, typename A)>
    member_function<mpl::list<R (CT::*)(BOOST_PP_ENUM_PARAMS(N, A)) const,R,member_function_tag,const_,CT const& BOOST_PP_ENUM_TRAILING_PARAMS(N, A)> >&
    function(const char* name,R (CT::*fn)(BOOST_PP_ENUM_PARAMS(N, A)) const) 
    {
        return derived().define_member(name,fn,boost::type<member_function<mpl::list<R (CT::*)(BOOST_PP_ENUM_PARAMS(N, A)) const,R,member_function_tag,const_,CT const& BOOST_PP_ENUM_TRAILING_PARAMS(N, A)> > >());
    }

    //Free function
    template<typename R BOOST_PP_ENUM_TRAILING_PARAMS(N, typename A)>
    member_function<mpl::list<R (*)(BOOST_PP_ENUM_PARAMS(N, A)),R,free_function_tag,cv_unqualified BOOST_PP_ENUM_TRAILING_PARAMS(N, A)> >&
    function(const char* name,R (*fn)(BOOST_PP_ENUM_PARAMS(N, A))) 
    {
        return derived().define_member(name,fn,boost::type<member_function<mpl::list<R (*)(BOOST_PP_ENUM_PARAMS(N, A)),R,free_function_tag,cv_unqualified BOOST_PP_ENUM_TRAILING_PARAMS(N, A)> > >());
    }

    //static Member function
    template<typename R,typename CT BOOST_PP_ENUM_TRAILING_PARAMS(N, typename A)>
    member_static_function<mpl::list<R (CT::*)(BOOST_PP_ENUM_PARAMS(N, A)),R,member_function_tag,cv_unqualified,CT& BOOST_PP_ENUM_TRAILING_PARAMS(N, A)> >&
    static_function(const char* name,R (CT::*fn)(BOOST_PP_ENUM_PARAMS(N, A))) 
    {
        return derived().define_member(name,fn,boost::type<member_static_function<mpl::list<R (CT::*)(BOOST_PP_ENUM_PARAMS(N, A)),R,member_function_tag,cv_unqualified,CT& BOOST_PP_ENUM_TRAILING_PARAMS(N, A)> > >());
    }

    //static Const member function
    template<typename R,typename CT BOOST_PP_ENUM_TRAILING_PARAMS(N, typename A)>
    member_static_function<mpl::list<R (CT::*)(BOOST_PP_ENUM_PARAMS(N, A)) const,R,member_function_tag,const_,CT const& BOOST_PP_ENUM_TRAILING_PARAMS(N, A)> >&
    static_function(const char* name,R (CT::*fn)(BOOST_PP_ENUM_PARAMS(N, A)) const) 
    {
        return derived().define_member(name,fn,boost::type<member_static_function<mpl::list<R (CT::*)(BOOST_PP_ENUM_PARAMS(N, A)) const,R,member_function_tag,const_,CT const& BOOST_PP_ENUM_TRAILING_PARAMS(N, A)> > >());
    }

    //static Free function
    template<typename R BOOST_PP_ENUM_TRAILING_PARAMS(N, typename A)>
    member_static_function<mpl::list<R (*)(BOOST_PP_ENUM_PARAMS(N, A)),R,free_function_tag,cv_unqualified BOOST_PP_ENUM_TRAILING_PARAMS(N, A)> >&
    static_function(const char* name,R (*fn)(BOOST_PP_ENUM_PARAMS(N, A))) 
    {
        return derived().define_member(name,fn,boost::type<member_static_function<mpl::list<R (*)(BOOST_PP_ENUM_PARAMS(N, A)),R,free_function_tag,cv_unqualified BOOST_PP_ENUM_TRAILING_PARAMS(N, A)> > >());
    }

#undef N

#endif //BOOST_PP_IS_ITERATING

