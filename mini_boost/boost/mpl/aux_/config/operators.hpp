
#ifndef BOOST_MPL_AUX_CONFIG_OPERATORS_HPP_INCLUDED
#define BOOST_MPL_AUX_CONFIG_OPERATORS_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2003-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /cvsroot-fuse/clipp/clipp/mini_boost/boost/mpl/aux_/config/operators.hpp,v $
// $Date: 2005/02/20 15:32:21 $
// $Revision: 1.1 $

#include <boost/mpl/aux_/config/gcc.hpp>
#include <boost/mpl/aux_/config/msvc.hpp>
#include <boost/mpl/aux_/config/workaround.hpp>

#if !defined(BOOST_MPL_CFG_USE_OPERATORS_OVERLOADING) \
    && ( BOOST_WORKAROUND(BOOST_MSVC, <= 1300) \
        || BOOST_WORKAROUND(__BORLANDC__, <= 0x600) \
        || BOOST_WORKAROUND(__EDG_VERSION__, <= 245) \
        || BOOST_WORKAROUND(BOOST_MPL_CFG_GCC, <= 0x0295) \
        )

#   define BOOST_MPL_CFG_USE_OPERATORS_OVERLOADING

#endif

#endif // BOOST_MPL_AUX_CONFIG_OPERATORS_HPP_INCLUDED
