#include <boost/javascript/javascript_grammar.hpp>
#include <boost/javascript/literal_directive.hpp>
#include <boost/javascript/parse_guard.hpp>

using namespace boost::javascript;

void javascript_grammar::definition<javascript_grammar::scanner_t>::define_statements()
{
// javascript identifiers
    variable_statement = root_node_d[literal_d[str_p("var")]] >> parse_guard_d[variable_declaration_list] >> discard_node_d[optional_semicolon];
    variable_declaration_list = infix_node_d[parse_guard_d[variable_declaration] >> *(',' >> parse_guard_d[variable_declaration])];
    variable_declaration_list_no_in = infix_node_d[parse_guard_d[variable_declaration_no_in] >> *(',' >> parse_guard_d[variable_declaration_no_in])];
    variable_declaration = identifier >> !(root_node_d[ch_p('=')] >> parse_guard_d[assignment_expression]);
    variable_declaration_no_in = identifier >> !(root_node_d[ch_p('=')] >> parse_guard_d[assignment_expression_no_in]);
    empty_statement = root_node_d[ch_p(';')];
    expression_statement = expression >> discard_node_d[optional_semicolon];
    expression_statement_no_in = discard_last_node_d[expression_no_in >> parse_guard_d[ch_p(';')]];
    if_statement 
        = root_node_d[literal_d[str_p("if")]] >> inner_node_d[parse_guard_d[ch_p('(')] >> parse_guard_d[expression] >> parse_guard_d[ch_p(')')]] >> parse_guard_d[statement] >> !discard_first_node_d[(literal_d[str_p("else")] >> parse_guard_d[statement])];
    do_while_statement 
        = root_node_d[literal_d[str_p("do")]] >> parse_guard_d[statement] >> parse_guard_d[no_node_d[literal_d[str_p("while")]]] >> inner_node_d[parse_guard_d[ch_p('(')] >> parse_guard_d[expression] >> parse_guard_d[ch_p(')')]] >> discard_node_d[optional_semicolon];
        
}
