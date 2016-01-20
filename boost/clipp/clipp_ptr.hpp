#if !defined (BOOST_PP_IS_ITERATING)

// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_PTR_HPP_HOLT_19122003
#define BOOST_CLIPP_PTR_HPP_HOLT_19122003

#include <boost/intrusive_ptr.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/clipp/config.hpp>
#include <vector>

namespace boost {namespace clipp {

class value;
template<typename T>
class clipp_ptr;

typedef clipp_ptr<value> valueP;
typedef std::vector<valueP> Params;


template<typename T>
class clipp_ptr : public intrusive_ptr<T> {
    typedef intrusive_ptr<T> this_type;
public:
    clipp_ptr(){}
    clipp_ptr(T * p, bool add_ref = true) :intrusive_ptr<T>(p,add_ref){}

#if !defined(BOOST_NO_MEMBER_TEMPLATES) || defined(BOOST_MSVC6_MEMBER_TEMPLATES)
    template<class U> clipp_ptr(clipp_ptr<U> const & rhs) :intrusive_ptr<T>(rhs) {}
#endif

    clipp_ptr(clipp_ptr const & rhs): intrusive_ptr<T>(rhs) {}

#if !defined(BOOST_NO_MEMBER_TEMPLATES) || defined(BOOST_MSVC6_MEMBER_TEMPLATES)
    template<class U> clipp_ptr & operator=(clipp_ptr<U> const & rhs)
    {        
        this_type(rhs).swap(*this);
        return *this;
    }
#endif

    clipp_ptr & operator=(clipp_ptr const & rhs)
    {
        this_type(rhs).swap(*this);
        return *this;
    }

    clipp_ptr & operator=(T * rhs)
    {
        this_type(rhs).swap(*this);
        return *this;
    }

    clipp_ptr operator[](const std::string& identifier) 
    {
        return get()->lookup(identifier);
    }

#define BOOST_PP_ITERATION_PARAMS_1 (3, (0, BOOST_CLIPP_MAX_ARITY, "boost/clipp/clipp_ptr.hpp"))
#include BOOST_PP_ITERATE()

};

}} //namespace boost::clipp

#endif //BOOST_CLIPP_PTR_HPP_HOLT_19122003

#else  //BOOST_PP_IS_ITERATING

#define N BOOST_PP_ITERATION()

#if N>0
    template<BOOST_PP_ENUM_PARAMS(N,typename A)>
#endif
    clipp_ptr operator()(BOOST_PP_ENUM_BINARY_PARAMS(N,A,const& a))
    {
        Params params(N);
        #define BOOST_CLIPP_TEXT(z,n,text) params[n]=wrap(BOOST_PP_CAT(a,n),get()->get_context());
        BOOST_PP_REPEAT(N, BOOST_CLIPP_TEXT, BOOST_PP_EMPTY) 
        #undef BOOST_CLIPP_TEXT
        return get()->call(params);
    }

#undef N

#endif //BOOST_PP_IS_ITERATING

