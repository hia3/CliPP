#if !defined (BOOST_PP_IS_ITERATING)

// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_CONSTRUCTOR_GENERATOR_EXPANDER_HPP_HOLT_24082003
#define BOOST_CLIPP_CONSTRUCTOR_GENERATOR_EXPANDER_HPP_HOLT_24082003

#include <boost/clipp/detail/file_iteration_helper.hpp>

namespace boost { namespace clipp { namespace detail {

template<typename Constructor>
struct constructor_generator {
    constructor_generator(Constructor constructor) : constructor_(constructor) {}
    typedef valueP result_type;

#define BOOST_CLIPP_ARGUMENT(z,n,text) argument<Signature,n> const& BOOST_PP_CAT(a,n)
#define BOOST_CLIPP_EXPAND_ARGUMENT(z,n,text) BOOST_PP_CAT(a,n)()

#define BOOST_PP_ITERATION_PARAMS_1 (3, (0, BOOST_CLIPP_MAX_ARITY, "boost/clipp/detail/constructor_generator.hpp"))
#include BOOST_PP_ITERATE()

#undef BOOST_CLIPP_ARGUMENT
#undef BOOST_CLIPP_EXPAND_ARGUMENT

    Constructor constructor_;
};

}}} //namespace boost::clipp::detail

#endif //BOOST_CLIPP_CONSTRUCTOR_GENERATOR_EXPANDER_HPP_HOLT_24082003

#else  //BOOST_PP_IS_ITERATING

#define N BOOST_PP_ITERATION()

//Constructor 
    template<typename R,typename cv_tagT BOOST_PP_ENUM_TRAILING_PARAMS( BOOST_PP_BOOL(N) , typename Signature BOOST_PP_INTERCEPT)>
    result_type execute(context* c,
                        boost::type<R>,
                        constructor_tag const&,
                        cv_tagT
                        BOOST_PP_ENUM_TRAILING( N , BOOST_CLIPP_ARGUMENT,BOOST_PP_EMPTY)
                       ) const
    {
        return wrap_struct<R*>::wrap(new R(BOOST_PP_ENUM(N,BOOST_CLIPP_EXPAND_ARGUMENT,BOOST_PP_EMPTY)),c);
    }
//Constructor no new
    template<typename R,typename cv_tagT BOOST_PP_ENUM_TRAILING_PARAMS( BOOST_PP_BOOL(N) , typename Signature BOOST_PP_INTERCEPT)>
    result_type execute(context* c,
                        boost::type<R>,
                        constructor_no_new_tag const&,
                        cv_tagT
                        BOOST_PP_ENUM_TRAILING( N , BOOST_CLIPP_ARGUMENT,BOOST_PP_EMPTY)
                       ) const
    {
        return wrap_struct<const R&>::wrap(R(BOOST_PP_ENUM(N,BOOST_CLIPP_EXPAND_ARGUMENT,BOOST_PP_EMPTY)),c);
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
        return wrap_struct<R>::wrap(constructor_(BOOST_PP_ENUM(N,BOOST_CLIPP_EXPAND_ARGUMENT,BOOST_PP_EMPTY)),c);
    }

#undef N

#endif //BOOST_PP_IS_ITERATING

