// Copyright Peder Holt 2003-2004. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_JAVASCRIPT_ERROR_HANDLER_HPP_HOLT_09052004
#define BOOST_JAVASCRIPT_ERROR_HANDLER_HPP_HOLT_09052004

#include <boost/javascript/config.hpp>
#include <boost/spirit/error_handling/exceptions.hpp>

namespace boost { namespace javascript {

struct javascript_error_handler {
    javascript_error_handler(const std::string& error_message_) : error_message(error_message_) {}
    javascript_error_handler(const char* error_message_) : error_message(error_message_) {}

    std::string error_message;
};

typedef boost::spirit::assertion<javascript_error_handler> javascript_assertion;
static javascript_assertion assert_function("Unexpected function declaration");
static javascript_assertion assert_literal("Expected literal");

}}

#endif //BOOST_JAVASCRIPT_ERROR_HANDLER_HPP_HOLT_09052004

