#if !defined (BOOST_PP_IS_ITERATING)

// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_SIGNATURE_HPP_HOLT_25082003
#define BOOST_CLIPP_SIGNATURE_HPP_HOLT_25082003

#include <boost/clipp/detail/file_iteration_helper.hpp>
#include <boost/clipp/detail/cv_category.hpp>
#include <boost/clipp/detail/list.hpp>
#include <boost/clipp/detail/pre_wrap_value.hpp>
#include <boost/clipp/unwrap.hpp>
#include <boost/clipp/arguments.hpp>
#include <boost/type_traits/is_base_and_derived.hpp>
#include <boost/ref.hpp>

#include <boost/mpl/or.hpp>

namespace boost { namespace clipp { namespace detail {

//Type tags
struct free_function_tag {};
struct free_data_tag {};
struct reference_wrapper_tag {};
struct constructor_tag {};
struct constructor_no_new_tag {};
struct member_function_tag {};
struct member_data_tag {};

template<typename CT>
struct implicit_conversion_tag{};

template<typename data_tag>
struct is_free_tag 
:   mpl::or_<
        is_same<data_tag,constructor_tag>,
        is_same<data_tag,free_function_tag>,
        is_same<data_tag,free_data_tag>
    >
{};

template<typename data_tag>
struct is_member_tag 
:   mpl::or_<
        is_same<data_tag,member_function_tag>,
        is_same<data_tag,member_data_tag>
    >
{};



#define BOOST_PP_ITERATION_PARAMS_1 (3, (0, BOOST_CLIPP_MAX_ARITY, "boost/clipp/detail/signature.hpp"))
#include BOOST_PP_ITERATE()

inline mpl::list<pre_wrap_value,pre_wrap_value,free_data_tag,cv_unqualified> 
get_signature(...) 
{
    return mpl::list<pre_wrap_value,pre_wrap_value,free_data_tag,cv_unqualified>();
}

inline free_data_tag get_data_tag(...)
{
    return free_data_tag();
}

template<typename T>
inline mpl::list<T&,T&,reference_wrapper_tag,cv_unqualified> 
get_signature(const reference_wrapper<T>&) 
{
    return mpl::list<T&,T&,reference_wrapper_tag,cv_unqualified>();
}

template<typename T>
inline reference_wrapper_tag get_data_tag(const reference_wrapper<T>&)
{
    return reference_wrapper_tag();
}

template<typename R BOOST_PP_ENUM_TRAILING_PARAMS(BOOST_CLIPP_MAX_ARITY, typename A)>
mpl::list<arguments<BOOST_PP_ENUM_PARAMS(BOOST_CLIPP_MAX_ARITY, A)>,R,constructor_tag,cv_unqualified BOOST_PP_ENUM_TRAILING_PARAMS(BOOST_CLIPP_MAX_ARITY, A)>
get_signature(arguments<BOOST_PP_ENUM_PARAMS(BOOST_CLIPP_MAX_ARITY, A)>,boost::type<R>)
{
    return mpl::list<arguments<BOOST_PP_ENUM_PARAMS(BOOST_CLIPP_MAX_ARITY, A)>,R,constructor_tag,cv_unqualified BOOST_PP_ENUM_TRAILING_PARAMS(BOOST_CLIPP_MAX_ARITY, A)>();
}

template<typename R BOOST_PP_ENUM_TRAILING_PARAMS(BOOST_CLIPP_MAX_ARITY, typename A)>
constructor_tag get_data_tag(arguments<BOOST_PP_ENUM_PARAMS(BOOST_CLIPP_MAX_ARITY, A)>,boost::type<R>)
{
    return constructor_tag();
}


template<typename R,typename CT>
mpl::list<R CT::*,R,member_data_tag,cv_unqualified,CT&,R>
get_signature(R CT::*var,...) 
{
    return mpl::list<R CT::*,R,member_data_tag,cv_unqualified,CT&,R>();
}

template<typename R,typename CT>
member_data_tag get_data_tag(R CT::*,R,member_data_tag,cv_unqualified,CT&,R)
{
    return member_data_tag();
}

template<typename T,typename CT>
mpl::list<type<T>,implicit_conversion_tag<CT>,cv_unqualified> 
get_signature(type<T>,type<T>,type<CT>)
{
    return mpl::list<type<T>,type<T>,implicit_conversion_tag<CT>,cv_unqualified>();
}

template<typename T,typename CT>
inline implicit_conversion_tag<CT> get_data_tag(type<T>,type<T>,type<CT>)
{
    return implicit_conversion_tag<CT>();
}

}}} //namespace boost::clipp::detail

#endif //BOOST_CLIPP_SIGNATURE_HPP_HOLT_25082003

#else  //BOOST_PP_IS_ITERATING

#define N BOOST_PP_ITERATION()

//free function
template<typename R BOOST_PP_ENUM_TRAILING_PARAMS(N, typename A)>
mpl::list<R (*)(BOOST_PP_ENUM_PARAMS(N, A)),R,free_function_tag,cv_unqualified BOOST_PP_ENUM_TRAILING_PARAMS(N, A)>
get_signature(R (*fn)(BOOST_PP_ENUM_PARAMS(N, A)),...)
{
    return mpl::list<R (*)(BOOST_PP_ENUM_PARAMS(N, A)),R,free_function_tag,cv_unqualified BOOST_PP_ENUM_TRAILING_PARAMS(N, A)>();
}

template<typename R BOOST_PP_ENUM_TRAILING_PARAMS(N, typename A)>
free_function_tag get_data_tag(R (*fn)(BOOST_PP_ENUM_PARAMS(N, A)),...)
{
    return free_function_tag();
}

//member function
template<typename R,typename CT BOOST_PP_ENUM_TRAILING_PARAMS(N, typename A)>
mpl::list<R (CT::*)(BOOST_PP_ENUM_PARAMS(N, A)),R,member_function_tag,cv_unqualified,CT& BOOST_PP_ENUM_TRAILING_PARAMS(N, A)>
get_signature(R (CT::*fn)(BOOST_PP_ENUM_PARAMS(N, A)),...)
{
    return mpl::list<R (CT::*)(BOOST_PP_ENUM_PARAMS(N, A)),R,member_function_tag,cv_unqualified,CT& BOOST_PP_ENUM_TRAILING_PARAMS(N, A)>();
}

template<typename R,typename CT BOOST_PP_ENUM_TRAILING_PARAMS(N, typename A)>
member_function_tag get_data_tag(R (CT::*fn)(BOOST_PP_ENUM_PARAMS(N, A)),...)
{
    return member_function_tag();
}

//const member function
template<typename R,typename CT BOOST_PP_ENUM_TRAILING_PARAMS(N, typename A)>
mpl::list<R (CT::*)(BOOST_PP_ENUM_PARAMS(N, A)) const,R,member_function_tag,const_,CT const& BOOST_PP_ENUM_TRAILING_PARAMS(N, A)>
get_signature(R (CT::*fn)(BOOST_PP_ENUM_PARAMS(N, A)) const,...)
{
    return mpl::list<R (CT::*)(BOOST_PP_ENUM_PARAMS(N, A)) const,R,member_function_tag,const_,CT const& BOOST_PP_ENUM_TRAILING_PARAMS(N, A)>();
}

template<typename R,typename CT BOOST_PP_ENUM_TRAILING_PARAMS(N, typename A)>
member_function_tag get_data_tag(R (CT::*fn)(BOOST_PP_ENUM_PARAMS(N, A)) const,...)
{
    return member_function_tag();
}

#undef N

#endif //BOOST_PP_IS_ITERATING

