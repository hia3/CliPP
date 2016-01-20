#if !defined (BOOST_PP_IS_ITERATING)

// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_REGISTER_TYPES_HPP_HOLT_24082003
#define BOOST_CLIPP_REGISTER_TYPES_HPP_HOLT_24082003

#include <boost/clipp/config.hpp>
#include <boost/clipp/value.hpp>
#include <boost/clipp/detail/signature_extracter.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum_trailing.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/facilities/empty.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/clipp/detail/register_type.hpp>

namespace boost { namespace clipp { namespace detail {

struct register_types {
    register_types() {}
    typedef bool result_type;

#define BOOST_CLIPP_COMMA() ,
#define BOOST_CLIPP_ARGUMENT(z,n,text) argument<Function_Traits,n> const& BOOST_PP_CAT(a,n)
#define BOOST_CLIPP_UNWIND_ARGUMENT_TYPE(z,n,text) register_traits(BOOST_CLIPP_UNWRAP_TYPE(BOOST_DEDUCED_TYPENAME argument<Function_Traits BOOST_CLIPP_COMMA() n>::result_type),c);

#define BOOST_PP_ITERATION_PARAMS_1 (3, (0, BOOST_CLIPP_MAX_ARITY, "boost/clipp/detail/register_types.hpp"))
#include BOOST_PP_ITERATE()

#undef BOOST_CLIPP_ARGUMENT
#undef BOOST_CLIPP_UNWIND_ARGUMENT_TYPE
#undef BOOST_CLIPP_COMMA

};

}}} //namespace boost::clipp::detail

#endif //BOOST_CLIPP_REGISTER_TYPES_HPP_HOLT_24082003

#else  //BOOST_PP_IS_ITERATING

#define N BOOST_PP_ITERATION()


    //Return traits
    template<typename R,typename fn_tagT,typename cv_tagT BOOST_PP_ENUM_TRAILING_PARAMS( BOOST_PP_BOOL(N) , typename Function_Traits BOOST_PP_INTERCEPT)>
    result_type execute(context* c,
                        boost::type<R>,
                        fn_tagT,
                        cv_tagT
                        BOOST_PP_ENUM_TRAILING( N , BOOST_CLIPP_ARGUMENT,BOOST_PP_EMPTY)
                       ) const
    {
        register_traits(BOOST_CLIPP_UNWRAP_TYPE(R),c);
        BOOST_PP_REPEAT(N,BOOST_CLIPP_UNWIND_ARGUMENT_TYPE,BOOST_PP_EMPTY);
        return true;
    }
    //Void traits
    template<typename fn_tagT,typename cv_tagT BOOST_PP_ENUM_TRAILING_PARAMS( BOOST_PP_BOOL(N) , typename Function_Traits BOOST_PP_INTERCEPT)>
    result_type execute(context* c,
                        mpl::void_,
                        fn_tagT,
                        cv_tagT
                        BOOST_PP_ENUM_TRAILING( N , BOOST_CLIPP_ARGUMENT,BOOST_PP_EMPTY)
                       ) const
    {
        BOOST_PP_REPEAT(N,BOOST_CLIPP_UNWIND_ARGUMENT_TYPE,BOOST_PP_EMPTY);
        return true;
    }

#undef N

#endif //BOOST_PP_IS_ITERATING

