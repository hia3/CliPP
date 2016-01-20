// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_INVALID_CONVERSION_HPP_HOLT_28092003
#define BOOST_CLIPP_INVALID_CONVERSION_HPP_HOLT_28092003

#include <boost/clipp/config.hpp>
#include <boost/clipp/is_check_helper.hpp>
#include <boost/preprocessor/punctuation/comma.hpp>
#include <boost/preprocessor/cat.hpp>

namespace boost { namespace clipp {

template<typename From,typename To>
struct invalid_conversion {
    typedef From from_type;
    typedef To   to_type;
};

BOOST_CLIPP_IS_2(invalid_conversion,invalid_conversion);

}} //namespace boost::clipp

#endif //BOOST_CLIPP_INVALID_CONVERSION_HPP_HOLT_28092003
