// Copyright Peder Holt 2003-2004. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_MESSAGE_HANDLER_HPP_HOLT_02052004
#define BOOST_MESSAGE_HANDLER_HPP_HOLT_02052004

#include <boost/javascript/callback_handler.hpp>
#include "boost/javascript/config.hpp"
#include "boost/clipp/scope.hpp"
#include <stdexcept>

namespace boost { namespace javascript {

//Keep for compatibility reasons.
class BOOST_JAVASCRIPT_EXPORT_IMPORT message_handler : public callback_handler
{
public:
    message_handler()  {}
    virtual ~message_handler() {}
};

class BOOST_JAVASCRIPT_EXPORT_IMPORT positional_runtime_error : public std::runtime_error
{
public:
    positional_runtime_error(const std::string& message,const parser_position& position) 
        : std::runtime_error(message) 
        , position_(position)
    {
    }
    virtual ~positional_runtime_error() {}
    const parser_position& parser_pos() const {return position_;}
private:
    parser_position position_;
};

class BOOST_JAVASCRIPT_EXPORT_IMPORT exception_handler : public message_handler
{
public:
    exception_handler()  {}
    virtual ~exception_handler() {}
    virtual void report_error(std::string const& message) const{
        throw positional_runtime_error(message,parser_pos());
    }
    virtual bool is_exception_handler() const {return true;}
};

class BOOST_JAVASCRIPT_EXPORT_IMPORT cerr_handler : public message_handler
{
public:
    cerr_handler()  {}
    virtual ~cerr_handler() {}
    virtual void report_error(std::string const& message) const{
        std::cerr << " In line " << parser_pos().get_current_line() << ": " << message;
    }

};

}} // namespace boost::javascript

#endif //BOOST_MESSAGE_HANDLER_HPP_HOLT_02052004

