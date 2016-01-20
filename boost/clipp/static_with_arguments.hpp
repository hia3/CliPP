#if !defined (BOOST_PP_IS_ITERATING)

// Copyright Peder Holt 2003-2004. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_STATIC_WITH_ARGUMENTS_HPP_HOLT_29022004
#define BOOST_CLIPP_STATIC_WITH_ARGUMENTS_HPP_HOLT_29022004

#include <boost/clipp/member_base.hpp>

#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum_trailing_binary_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/facilities/empty.hpp>
#include <boost/clipp/detail/register_types.hpp>

namespace boost { namespace clipp {

template<unsigned ArityI>
struct static_with_arguments
{
};

#define BOOST_PP_ITERATION_PARAMS_1 (3, (0, BOOST_CLIPP_MAX_ARITY, "boost/clipp/static_with_arguments.hpp"))
#include BOOST_PP_ITERATE()

}} //namespace boost::clipp

#endif //BOOST_CLIPP_STATIC_WITH_ARGUMENTS_HPP_HOLT_29022004

#else //BOOST_PP_IS_ITERATING

#define N BOOST_PP_ITERATION()

template<>
struct static_with_arguments<N> 
{
    template<typename Signature,typename DerivedT>
    class inner : public member_base<DerivedT> {
    public:
        typedef Signature signature_type;
        BOOST_STATIC_CONSTANT(int,arity=N);
        int get_arity() {return N;}

        inner(const char* name)
        :   member_base<DerivedT>(name)
        {}

        ///Generic invocation of arguments.
        template<typename ArgumentListT,typename GeneratorT>
        typename GeneratorT::result_type
        invoke(const ArgumentListT& arguments,GeneratorT const& generator=NULL)
        {
            return generator.execute(
                get_context(),
                detail::return_type_wrapper<Signature>::type(),
                detail::data_type<Signature>::type(),
                detail::cv_type<Signature>::type()
                #define BOOST_CLIPP_TEXT(z,n,text) detail::argument<Signature,n>(arguments)
                BOOST_PP_ENUM_TRAILING(N,BOOST_CLIPP_TEXT,BOOST_PP_EMPTY)
                #undef  BOOST_CLIPP_TEXT
            );
        }
        virtual void create(context* c) {
            member_base<DerivedT>::create(c);
            invoke(empty_argument_list(),detail::register_types());
        }

        //Define the signature of this function/constructor
        DerivedT& signature(BOOST_PP_ENUM_PARAMS(N,arg const& a))
        {
            #define BOOST_CLIPP_TEXT(z,n,text) BOOST_PP_CAT(_a,n).signature(BOOST_PP_CAT(a,n));
            BOOST_PP_REPEAT(N,BOOST_CLIPP_TEXT,BOOST_PP_EMPTY)
            #undef BOOST_CLIPP_TEXT
            return derived();
        }
        //Return a formatted argument list
        std::string format_argument_list()
        {    
            std::string format;
            #define BOOST_CLIPP_TEXT(z,n,text) format += BOOST_PP_IF(n,", ","") + BOOST_PP_CAT(_a,n).format(get_context());
            BOOST_PP_REPEAT(N,BOOST_CLIPP_TEXT,BOOST_PP_EMPTY)
            #undef BOOST_CLIPP_TEXT
            return format;
        }
        //Get the default value at a given position.
        valueP get_default_value(size_t index) {
            #define BOOST_CLIPP_TEXT(z,n,text) if(index==n) return BOOST_PP_CAT(_a,n).get_default_value(get_context());
            BOOST_PP_REPEAT(N,BOOST_CLIPP_TEXT,BOOST_PP_EMPTY)
            #undef  BOOST_CLIPP_TEXT
            return NULL;
        }

    private:        
#define BOOST_CLIPP_TEXT(z,n,text) doc_arg<Signature,n> BOOST_PP_CAT(_a,n);
        BOOST_PP_REPEAT(N,BOOST_CLIPP_TEXT,BOOST_PP_EMPTY)
#undef  BOOST_CLIPP_TEXT
    };    
};

#undef N

#endif //BOOST_PP_IS_ITERATING

