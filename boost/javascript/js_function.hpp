// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_JAVASCRIPT_FUNCTION_HPP_HOLT_20032004
#define BOOST_JAVASCRIPT_FUNCTION_HPP_HOLT_20032004

#include "boost/javascript/config.hpp"
#include "boost/clipp/object.hpp"
#include "boost/javascript/callback_handler.hpp"
#include <valarray>
namespace boost { namespace javascript {

struct function_info;

class BOOST_JAVASCRIPT_EXPORT_IMPORT js_function : public clipp::object  
{
public:
    js_function(std::valarray<std::string>& args);
    js_function(const std::string& args,const std::string& program,const callback_handler& handler=callback_handler());
    virtual ~js_function();
    virtual clipp::valueP call(clipp::Params& args,clipp::valueP parent=NULL);
    virtual clipp::valueP construct(clipp::Params& args,clipp::valueP parent=NULL);
    std::string toString() const;
    const std::string& program() const {return program_;}
    virtual clipp::valueP wrap_value(clipp::valueP parent);

    static void init(clipp::context* c);
private:
    std::string arguments_;
    std::string program_;
    function_info* function_info_;
    parser_position parser_pos_;
};

}} // namespace boost::clipp

#endif //BOOST_JAVASCRIPT_FUNCTION_HPP_HOLT_20032004

