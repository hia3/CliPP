#if !defined (BOOST_PP_IS_ITERATING)

// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_ARGUMENT_VALIDATOR_EXPANDER_HPP_HOLT_24082003
#define BOOST_CLIPP_ARGUMENT_VALIDATOR_EXPANDER_HPP_HOLT_24082003

#include <boost/clipp/detail/file_iteration_helper.hpp>

namespace boost { namespace clipp { namespace detail {

struct argument_validator {
    typedef bool result_type;

#define BOOST_CLIPP_ARGUMENT(z,n,text) argument<Signature,n> const& BOOST_PP_CAT(a,n)
#define BOOST_CLIPP_IS_OK(z,n,text) && BOOST_PP_CAT(a,n.ok())

#define BOOST_PP_ITERATION_PARAMS_1 (3, (0, BOOST_CLIPP_MAX_ARITY, "boost/clipp/detail/argument_validator.hpp"))
#include BOOST_PP_ITERATE()

#undef BOOST_CLIPP_ARGUMENT
#undef BOOST_CLIPP_IS_OK

};

}}} //namespace boost::clipp::detail

#endif //BOOST_CLIPP_ARGUMENT_VALIDATOR_EXPANDER_HPP_HOLT_24082003

#else  //BOOST_PP_IS_ITERATING

#define N BOOST_PP_ITERATION()

    template<typename R,typename CT,typename CV BOOST_PP_ENUM_TRAILING_PARAMS( BOOST_PP_BOOL(N) , typename Signature BOOST_PP_INTERCEPT)>
    result_type execute(context* c,
                        R,
                        CT const&,
                        CV
                        BOOST_PP_ENUM_TRAILING( N , BOOST_CLIPP_ARGUMENT,BOOST_PP_EMPTY)
                       ) const
    {
        return true BOOST_PP_REPEAT(N,BOOST_CLIPP_IS_OK,BOOST_PP_EMPTY);
    }

#undef N

#endif //BOOST_PP_IS_ITERATING

