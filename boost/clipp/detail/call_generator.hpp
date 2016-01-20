#if !defined (BOOST_PP_IS_ITERATING)

// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_CALL_GENERATOR_EXPANDER_HPP_HOLT_17082003
#define BOOST_CLIPP_CALL_GENERATOR_EXPANDER_HPP_HOLT_17082003

#include <boost/clipp/detail/file_iteration_helper.hpp>

namespace boost { namespace clipp { namespace detail {

template<typename Fn>
struct call_generator {
    call_generator(Fn fn) : fn_(fn) {}
    typedef valueP result_type;

#define BOOST_CLIPP_ARGUMENT(z,n,text) argument<Signature,n> const& BOOST_PP_CAT(a,n)
#define BOOST_CLIPP_EXPAND_ARGUMENT(z,n,text) BOOST_PP_CAT(a,n)()
#define BOOST_CLIPP_EXPAND_MEMBER_ARGUMENT(z,n,text) BOOST_PP_CAT(a,BOOST_PP_INC(n))()

#define BOOST_PP_ITERATION_PARAMS_1 (3, (0, BOOST_CLIPP_MAX_ARITY, "boost/clipp/detail/call_generator.hpp"))
#include BOOST_PP_ITERATE()

#undef BOOST_CLIPP_ARGUMENT
#undef BOOST_CLIPP_EXPAND_ARGUMENT
#undef BOOST_CLIPP_EXPAND_MEMBER_ARGUMENT

    Fn fn_;
};

}}} //namespace boost::clipp::detail

#endif //BOOST_CLIPP_CALL_GENERATOR_EXPANDER_HPP_HOLT_17082003

#else  //BOOST_PP_IS_ITERATING

#define N BOOST_PP_ITERATION()
#define N_1 BOOST_PP_DEC(N)


//Member function    
    template<typename R,typename cv_tagT BOOST_PP_ENUM_TRAILING_PARAMS( BOOST_PP_BOOL(N) , typename Signature BOOST_PP_INTERCEPT)>
    result_type execute(context* c,
                        boost::type<R>,
                        member_function_tag const& ct,
                        cv_tagT
                        BOOST_PP_ENUM_TRAILING( N , BOOST_CLIPP_ARGUMENT,BOOST_PP_EMPTY)
                       ) const
    {
        return wrap_struct<R>::wrap((a0().*fn_)(BOOST_PP_ENUM(N_1,BOOST_CLIPP_EXPAND_MEMBER_ARGUMENT,BOOST_PP_EMPTY)),c);
    }
//Void member function
    template<typename cv_tagT BOOST_PP_ENUM_TRAILING_PARAMS( BOOST_PP_BOOL(N) , typename Signature BOOST_PP_INTERCEPT)>
    result_type execute(context* c,
                        mpl::void_,
                        member_function_tag const& ct,
                        cv_tagT
                        BOOST_PP_ENUM_TRAILING( N , BOOST_CLIPP_ARGUMENT,BOOST_PP_EMPTY)
                       ) const
    {
        (a0().*fn_)(BOOST_PP_ENUM(N_1,BOOST_CLIPP_EXPAND_MEMBER_ARGUMENT,BOOST_PP_EMPTY));
        return NULL;
    }

//Free function    
    template<typename R,typename cv_tagT BOOST_PP_ENUM_TRAILING_PARAMS( BOOST_PP_BOOL(N) , typename Signature BOOST_PP_INTERCEPT)>
    result_type execute(context* c,
                        boost::type<R>,
                        free_function_tag const&,
                        cv_tagT
                        BOOST_PP_ENUM_TRAILING( N , BOOST_CLIPP_ARGUMENT,BOOST_PP_EMPTY)
                       ) const
    {
        return wrap_struct<R>::wrap(fn_(BOOST_PP_ENUM(N,BOOST_CLIPP_EXPAND_ARGUMENT,BOOST_PP_EMPTY)),c);
    }
//Void free function
    template<typename cv_tagT BOOST_PP_ENUM_TRAILING_PARAMS( BOOST_PP_BOOL(N) , typename Signature BOOST_PP_INTERCEPT)>
    result_type execute(context* c,
                        mpl::void_,
                        free_function_tag const&,
                        cv_tagT
                        BOOST_PP_ENUM_TRAILING( N , BOOST_CLIPP_ARGUMENT,BOOST_PP_EMPTY)
                       ) const
    {
        fn_(BOOST_PP_ENUM(N,BOOST_CLIPP_EXPAND_ARGUMENT,BOOST_PP_EMPTY));
        return NULL;
    }

#undef N
#undef N_1

#endif //BOOST_PP_IS_ITERATING

