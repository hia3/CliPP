#include "boost/spirit/core.hpp"
#include "boost/spirit/tree/ast.hpp"
#include "boost/javascript/javascript_parser.hpp"
#include "boost/javascript/javascript_grammar.hpp"
#include <boost/clipp/context.hpp>
#include <boost/spirit/error_handling/exceptions.hpp>

using namespace std;
using namespace boost::javascript;
using namespace boost::clipp;

valueP javascript_parser::parse(iterator_t const& first,iterator_t const& last,callback_handler const& handler)
{
    m_handler=&handler;
    try {
        handler.parser_pos().set_range(first,last);
        javascript_grammar grammar;
        skip_grammar skip;
        tree_parse_t info = ast_parse<factory_t>(first, last, grammar, skip);
        dump(info);
        if (info.full) {
            return evaluate(info,handler);
        }
        else {
            handler.parser_pos().set_current(info.stop);
            std::string message = " Fails Parsing\n";
            for (int i = 0; i < 50; i++)
            {
                if (info.stop == last)
                    break;
                message += *info.stop++;
            }
            message +="\n";
            throw parsing_incomplete(message);
        }
    }
    catch(parse_guard_triggered& e) {
        m_handler=0;
        handler.parser_pos().set_current(e.where());
        if(handler.is_exception_handler()) throw;
        else {
            std::string message = " Syntax error: ";
            message+=e.what();
            handler.report_error(message);
            return 0;
        }
    }
    catch(positional_runtime_error& e) {
        m_handler=0;
        handler.parser_pos()=e.parser_pos();
        if(handler.is_exception_handler()) throw;
        else {
            handler.report_error(e.what());
            return 0;
        }
    }
    catch(boost::spirit::parser_error<iterator_t, iterator_t>& e) {
        m_handler=0;
        handler.parser_pos().set_current(e.where);
        if(handler.is_exception_handler()) throw;
        else {
            handler.report_error(e.descriptor);
            return 0;
        }
    }
    catch(exception& e) {
        m_handler=0;
        if(handler.is_exception_handler()) throw;
        else {
            handler.report_error(e.what());
            return 0;
        }
    }

    catch(...) {
        m_handler=0;
        handler.report_error("A parsing error occured\n");
        return 0;
    }
    m_handler=0;
}

