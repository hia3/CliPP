#include <boost/javascript/javascript_grammar.hpp>
#include <boost/javascript/literal_directive.hpp>
#include <boost/javascript/no_eol_directive.hpp>
#include <boost/javascript/parse_guard.hpp>

using namespace boost::javascript;

void javascript_grammar::definition<javascript_grammar::scanner_t>::define_grammar9()
{
    literal
        = null_literal
        | boolean_literal
        | numeric_literal
        | string_literal
        | array_literal
        | inner_node_d['(' >> parse_guard_d[expression] >> parse_guard_d[ch_p(')')]]
        ;
    
    return_statement = root_node_d[literal_d[str_p("return")]] >> !(discard_node_d[no_eol_d[eps_p(anychar_p - eol_p)]] >> expression) >> discard_node_d[optional_semicolon];
    source_element = function_declaration | statement;
    statement_list = *statement;
    block = inner_node_d['{' >> statement_list >> parse_guard_d[ch_p('}')]];
}

