#if !defined (BOOST_PP_IS_ITERATING)

// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_TYPE_LIST_HPP_HOLT_13082003
#define BOOST_CLIPP_TYPE_LIST_HPP_HOLT_13082003

#include <boost/preprocessor/iteration/iterate.hpp>
#include "boost/preprocessor/enum_params.hpp"
#include <boost/clipp/config.hpp>
#include <boost/clipp/detail/cv_category.hpp>
#include <boost/clipp/detail/list.hpp>
#include <boost/mpl/void.hpp>

namespace boost { namespace clipp { namespace detail {

template<int N > struct type_list_selector;

#define BOOST_CLIPP_IS_VOID(z,n,text) BOOST_PP_IF(n,+ mpl::is_void_,mpl::is_void_)<A##n>::value

#define BOOST_PP_ITERATION_PARAMS_1 (3, (0, BOOST_CLIPP_MAX_ARITY, "boost/clipp/detail/type_list.hpp"))
#include BOOST_PP_ITERATE()

template<BOOST_PP_ENUM_PARAMS(BOOST_CLIPP_MAX_ARITY, typename A)>
struct list_count_args
{
    BOOST_STATIC_CONSTANT(int,value = (BOOST_PP_REPEAT(BOOST_CLIPP_MAX_ARITY,BOOST_CLIPP_IS_VOID,BOOST_PP_EMPTY)));
};

template<BOOST_PP_ENUM_PARAMS(BOOST_CLIPP_MAX_ARITY, typename A)>
struct type_list : type_list_selector<list_count_args<BOOST_PP_ENUM_PARAMS(BOOST_CLIPP_MAX_ARITY, A)>::value>::
                   result_<BOOST_PP_ENUM_PARAMS(BOOST_CLIPP_MAX_ARITY, A)>::type
{
};

#undef BOOST_CLIPP_IS_VOID

}}} //namespace boost::clipp::detail

//Generates functions of the following syntax:
//template<typename R,typename A1..typename AN>
//function_traits<R,fn_tag,mpl_list<A1..AN>,cv_tag<false,false> >
//get_function_traits(R (*fn)(A1..AN))
//{
//    return function_traits<R,fn_tag,mpl_list<A1..AN>,cv_tag<false,false> >(fn);
//}

#endif //BOOST_CLIPP_TYPE_LIST_HPP_HOLT_13082003

#else  //BOOST_PP_IS_ITERATING

#define N BOOST_PP_ITERATION()

template<>
struct type_list_selector<N>
{
    template<BOOST_PP_ENUM_PARAMS(BOOST_CLIPP_MAX_ARITY, typename A)>
    struct result_ {
        typedef mpl::list<BOOST_PP_ENUM_PARAMS(N,A)> type;
    };
};

#endif //BOOST_PP_IS_ITERATING
