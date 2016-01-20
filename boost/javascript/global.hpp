// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_JAVASCRIPT_GLOBAL_HPP_HOLT_29092003
#define BOOST_JAVASCRIPT_GLOBAL_HPP_HOLT_29092003

#include <boost/javascript/config.hpp>
#include <boost/clipp/scope.hpp>
#include <string>

namespace boost { namespace javascript {

class BOOST_JAVASCRIPT_EXPORT_IMPORT global : public clipp::scope  
{
public:
    global() {}
    virtual ~global() {}
    clipp::valueP eval(const std::string& x);
    static double parseInt(const std::string& number,int radix=10);
    static double parseFloat(const std::string& number);
    static void init(clipp::context* c);
private:
};

}} // namespace boost::clipp

#endif //BOOST_JAVASCRIPT_GLOBAL_HPP_HOLT_29092003

