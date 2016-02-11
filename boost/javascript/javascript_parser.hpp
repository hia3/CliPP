// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef JAVASCRIPT_JAVASCRIPT_PARSER_HPP_HOLT_2003_0103
#define JAVASCRIPT_JAVASCRIPT_PARSER_HPP_HOLT_2003_0103

#include <boost/javascript/config.hpp>
//#include <boost/spirit/core.hpp>
//#include <boost/spirit/tree/ast.hpp>
#include <boost/clipp/value.hpp>
#include <boost/clipp/context.hpp>
#include <boost/javascript/message_handler.hpp>
#include <fstream>
#include <stack>
#include <list>

namespace boost { namespace spirit {
    template <typename ValueT>
    class node_iter_data_factory;

    struct nil_t;

    template <typename IteratorT,typename NodeFactoryT,typename T>
    struct tree_parse_info;
}} //namespace boost::spirit

namespace boost { namespace javascript {

class js_exit_return : public std::exception {
public:
    js_exit_return(clipp::valueP return_value) : return_value_(return_value) {}
    virtual ~js_exit_return() throw() {}
    const char* what() const throw() {return "Invalid context for return statement";}
    clipp::valueP return_value() {return return_value_;}
private:
    clipp::valueP return_value_;
};

class js_continue : public std::exception {
public:
    const char* what() const throw() {return "Invalid context for continue statement";}
};

class js_break : public std::exception {
public:
    const char* what() const throw() {return "Invalid context for break statement";}
};

class js_throw_statement : public std::exception {
public:
    js_throw_statement(clipp::valueP expression) : expression_(expression) {}
    virtual ~js_throw_statement() throw() {}
    const char* what() const noexcept {return "Javascript exception";}
    clipp::valueP expression() const {return expression_;}
private:
    clipp::valueP expression_;
};

class parsing_incomplete : public std::runtime_error {
public:
    parsing_incomplete(const std::string& error) : std::runtime_error(error) {}
    virtual ~parsing_incomplete() {}
};

class parse_guard_triggered : public std::runtime_error {
public:
    parse_guard_triggered(const std::string& error, const char* where) : std::runtime_error(error), where_(where) {}
    virtual ~parse_guard_triggered() {}
    const char* where() const { return where_; }
private:
    const char* where_;
};

class BOOST_JAVASCRIPT_EXPORT_IMPORT javascript_parser
:   public boost::clipp::context
{
public:
    javascript_parser();
    virtual ~javascript_parser();
    typedef char const* iterator_t;
    clipp::valueP parse(std::ifstream& file,callback_handler const& handler=cerr_handler());
    clipp::valueP parse(const char* str,callback_handler const& handler=cerr_handler());
    clipp::valueP parse(std::string const& str,callback_handler const& handler=cerr_handler()) {return parse(str.c_str(),handler);}

    clipp::valueP parse(iterator_t const& first,iterator_t const& last,callback_handler const& handler=cerr_handler());

    typedef spirit::node_iter_data_factory<double> factory_t;
//    typedef spirit::tree_match<iterator_t,factory_t, spirit::nil_t> parse_tree_match_t;
//    typedef parse_tree_match_t::tree_iterator iter_t;
    typedef spirit::tree_parse_info<iterator_t,factory_t, spirit::nil_t> tree_parse_t;
    clipp::context* get_context() {return this;}

    iterator_t code_begin() { assert(code_begin_ != nullptr); return code_begin_; }
    iterator_t code_begin_ = nullptr;
public:
    void push_all_scope(boost::clipp::valueP value);
    void push_lookup_scope(boost::clipp::valueP value);
    void pop_all_scope();
    void pop_lookup_scope();
    boost::clipp::valueP declaration_scope();
    boost::clipp::valueP lookup_scope();
    void top_stack_only(bool status);
    bool top_stack_only() const;
    clipp::valueP evaluate(tree_parse_t & info,callback_handler const& handler=cerr_handler());
    const callback_handler* get_callback_handler() { return m_handler; }
protected:
    void dump(tree_parse_t & info);
private:
    boost::clipp::valueP lookup_scope_;
    bool top_stack_only_;
    std::list<boost::clipp::valueP> lookup_scope_stack_;
    std::stack<boost::clipp::valueP> declaration_scope_stack_;
    mutable callback_handler const* m_handler;
};

class scope_pusher_all {
public:
    scope_pusher_all(javascript_parser* parser,boost::clipp::valueP value) : parser_(parser)
    {
        parser_->push_all_scope(value);
    }
    virtual ~scope_pusher_all() {
        parser_->pop_all_scope();
    }
private:
    javascript_parser* parser_;
};

class lookup_scope_top_stack {
public:
    lookup_scope_top_stack(javascript_parser* parser) : parser_(parser)
    {
        parser_->top_stack_only(true);
    }
    virtual ~lookup_scope_top_stack() {
        parser_->top_stack_only(false);
    }
private:
    javascript_parser* parser_;
};

}} //namespace boost::javascript

#endif //JAVASCRIPT_JAVASCRIPT_PARSER_HPP_HOLT_2003_0103

