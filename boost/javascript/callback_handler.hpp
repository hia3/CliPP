#ifndef BOOST_CALLBACK_HANDLER_HPP_WOLFF_26082005
#define BOOST_CALLBACK_HANDLER_HPP_WOLFF_26082005

#include <boost/javascript/config.hpp>
#include <boost/clipp/scope.hpp>
#include <boost/javascript/parser_position.hpp>
#include <stdexcept>

namespace boost { namespace javascript {

class js_branch_callback : public std::exception {
public:
    const char* what() const throw(){return "Script stopped by user";}
};


class BOOST_JAVASCRIPT_EXPORT_IMPORT callback_handler
{
public:
    callback_handler() {}
    virtual ~callback_handler() {}
    virtual void report_error(std::string const& message) const{}
    virtual bool branch_callback() const {return true;}
    virtual void branch_call() const
    {
        if (!branch_callback())
        {
            throw js_branch_callback();
        }
    }
    virtual bool is_exception_handler() const {return false;}
    parser_position& parser_pos() const { return parser_pos_; }
private:
    mutable parser_position parser_pos_;
};

}} // namespace boost::javascript

#endif //BOOST_CALLBACK_HANDLER_HPP_WOLFF_26082005

