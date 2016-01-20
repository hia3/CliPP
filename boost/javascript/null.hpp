// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_JAVASCRIPT_NULL_HPP_HOLT_16112003
#define BOOST_JAVASCRIPT_NULL_HPP_HOLT_16112003

#include "boost/javascript/config.hpp"
#include "boost/clipp/object.hpp"

namespace boost { namespace javascript {

class BOOST_JAVASCRIPT_EXPORT_IMPORT null : public clipp::object  
{
public:
    virtual ~null() {}
    null()  {}
    static void init(clipp::context* c);
    bool operator ==(const null &) const {return true;}
    bool operator ==(const clipp::value &) const {return false;}
};

}} // namespace boost::clipp

#endif //BOOST_JAVASCRIPT_UNDEFINED_HPP_HOLT_15092003

