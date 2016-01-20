// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_DETAIL_DECORATED_WRAPPER_HPP_HOLT_14092003
#define BOOST_CLIPP_DETAIL_DECORATED_WRAPPER_HPP_HOLT_14092003

#include <boost/clipp/config.hpp>
#include <boost/clipp/decoration.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/clipp/wrap.hpp>

namespace boost { namespace clipp { namespace detail {
    
struct wrapper_base;
typedef intrusive_ptr<wrapper_base> wrapperP;

class decorated_wrapper {
public:
    decorated_wrapper() {}
    void     add_wrapper(wrapperP c);
    wrapperP get_wrapper(decoration d);
private:
    wrapperP wrappers_[6];
};

}}} //namespace boost::clipp::detail

#endif //BOOST_CLIPP_DETAIL_DECORATED_WRAPPER_HPP_HOLT_14092003

