#if !defined (BOOST_PP_IS_ITERATING)

// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_SIGNATURE_INVOKER_HPP_HOLT_30082003
#define BOOST_CLIPP_SIGNATURE_INVOKER_HPP_HOLT_30082003

#include <boost/clipp/detail/file_iteration_helper.hpp>

namespace boost { namespace clipp { namespace detail {

template<int arity>
struct signature_invoker {
    template<typename Signature,typename Generator>
    static typename Generator::result_type
    invoke(const value::Params& params,value* parent,Generator* generator=NULL,Signature* dummy1=NULL);
};

#define BOOST_CLIPP_ARGUMENT(z,n,text) argument<Signature,n>(params)
#define BOOST_CLIPP_LAST_ARGUMENT(z,n,text) last_argument<Signature>(params)

#define BOOST_PP_ITERATION_PARAMS_1 (3, (0, BOOST_CLIPP_MAX_ARITY, "boost/clipp/detail/signature_invoker.hpp"))
#include BOOST_PP_ITERATE()

#undef BOOST_CLIPP_ARGUMENT
#undef BOOST_CLIPP_LAST_ARGUMENT

}}} //namespace boost::clipp::detail

#endif //BOOST_CLIPP_SIGNATURE_INVOKER_HPP_HOLT_30082003

#else  //BOOST_PP_IS_ITERATING


#define N BOOST_PP_ITERATION()
#define N_1 BOOST_PP_SUB(N, BOOST_PP_BOOL(N) )

template<>
struct signature_invoker<N> {
    template<typename Signature,typename Generator,typename ArgumentListT>
    static typename Generator::result_type
    invoke(context* c,const ArgumentListT& params,Generator* generator=NULL,Signature* dummy1=NULL)
    {
        return generator->execute(
            c,
            return_type_wrapper<Signature>::type(),
            data_type<Signature>::type(),
            cv_type<Signature>::type()
            BOOST_PP_ENUM_TRAILING( N_1 , BOOST_CLIPP_ARGUMENT,BOOST_PP_EMPTY)
            BOOST_PP_ENUM_TRAILING( BOOST_PP_BOOL(N) , BOOST_CLIPP_LAST_ARGUMENT,BOOST_PP_EMPTY)
        );
    }
};

#undef N_1
#undef N

#endif //BOOST_PP_IS_ITERATING