// Copyright Peder Holt 2003-2004. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_POLICY_TAG_HPP_HOLT_27052004
#define BOOST_CLIPP_POLICY_TAG_HPP_HOLT_27052004

#include <boost/clipp/detail/signature.hpp>

namespace boost { namespace clipp {

#if BOOST_WORKAROUND(BOOST_MSVC, <= 1300)

namespace aux {

#define AUX778076_MSVC_DTW_NAME msvc_apply
#define AUX778076_MSVC_DTW_ORIGINAL_NAME apply
#define AUX778076_MSVC_DTW_ARITY 1
#include <boost/mpl/aux_/msvc_dtw.hpp>

}

template< typename PolicyT, typename T >
struct policy_apply
{
    typedef typename aux::msvc_apply<PolicyT>
        ::template result_<T>::type type;
};

#else
template< typename policy, typename T > 
struct policy_apply
{
    typedef typename policy
        ::template apply<T>::type type;
};

#endif

struct policy_tag {};
struct storage_policy_tag : policy_tag {};

struct default_storage_policy : storage_policy_tag
{
    template<typename T>
    struct apply {
        typedef T type;
    };
    typedef detail::constructor_tag create_tag;
};

}} // namespace boost::clipp

#endif //BOOST_CLIPP_POLICY_TAG_HPP_HOLT_27052004

