
#ifndef BOOST_MPL_AUX_CONFIG_PREPROCESSOR_HPP_INCLUDED
#define BOOST_MPL_AUX_CONFIG_PREPROCESSOR_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /cvsroot-fuse/clipp/clipp/mini_boost/boost/mpl/aux_/config/preprocessor.hpp,v $
// $Date: 2005/02/20 15:32:21 $
// $Revision: 1.4 $

#include <boost/mpl/aux_/config/workaround.hpp>

#if !defined(BOOST_MPL_CFG_BROKEN_PP_MACRO_EXPANSION) \
    && (   BOOST_WORKAROUND(__MWERKS__, <= 0x3003) \
        || BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x561)) \
        || BOOST_WORKAROUND(__IBMCPP__, BOOST_TESTED_AT(502)) \
        )

#   define BOOST_MPL_CFG_BROKEN_PP_MACRO_EXPANSION

#endif

#if !defined(BOOST_MPL_CFG_NO_OWN_PP_PRIMITIVES)
#   define BOOST_MPL_CFG_NO_OWN_PP_PRIMITIVES
#endif

#if !defined(BOOST_NEEDS_TOKEN_PASTING_OP_FOR_TOKENS_JUXTAPOSING) \
    && BOOST_WORKAROUND(__DMC__, BOOST_TESTED_AT(0x840))
#   define BOOST_NEEDS_TOKEN_PASTING_OP_FOR_TOKENS_JUXTAPOSING
#endif


#endif // BOOST_MPL_AUX_CONFIG_PREPROCESSOR_HPP_INCLUDED
