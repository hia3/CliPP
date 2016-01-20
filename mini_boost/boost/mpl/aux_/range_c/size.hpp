
#ifndef BOOST_MPL_AUX_RANGE_C_SIZE_HPP_INCLUDED
#define BOOST_MPL_AUX_RANGE_C_SIZE_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /cvsroot-fuse/clipp/clipp/mini_boost/boost/mpl/aux_/range_c/size.hpp,v $
// $Date: 2005/02/20 15:34:30 $
// $Revision: 1.3 $

#include <boost/mpl/size_fwd.hpp>
#include <boost/mpl/minus.hpp>
#include <boost/mpl/aux_/range_c/tag.hpp>

namespace boost { namespace mpl {

template<>
struct size_impl< aux::half_open_range_tag >
{
    template< typename Range > struct apply
        : minus<
              typename Range::finish
            , typename Range::start
            >
    {
    };
};

}}

#endif // BOOST_MPL_AUX_RANGE_C_SIZE_HPP_INCLUDED
