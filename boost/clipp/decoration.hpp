// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_DECORATION_HPP_HOLT_07092003
#define BOOST_CLIPP_DECORATION_HPP_HOLT_07092003

namespace boost { namespace clipp {
    
enum decoration {
    d_direct            = 0,
    d_const_direct      = 1,
    d_reference         = 2,
    d_const_reference   = 3,
    d_pointer           = 4,
    d_const_pointer     = 5,
};

enum precedence {
    p_create=0,
    p_function=1,
    p_unwrap=2,
};

}} //namespace boost::clipp

#endif //BOOST_CLIPP_DECORATION_HPP_HOLT_07092003
