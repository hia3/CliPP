// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_DECORATION_HPP_HOLT_07092003
#define BOOST_CLIPP_DECORATION_HPP_HOLT_07092003

namespace boost { namespace clipp {
    
enum class decoration : unsigned {
    direct            = 0,
    const_direct      = 1,
    reference         = 2,
    const_reference   = 3,
    pointer           = 4,
    const_pointer     = 5,
};

enum class precedence {
    create=0,
    function=1,
    unwrap=2,
};

}} //namespace boost::clipp

#endif //BOOST_CLIPP_DECORATION_HPP_HOLT_07092003
