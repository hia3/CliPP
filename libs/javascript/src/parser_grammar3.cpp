#include <boost/javascript/javascript_grammar.hpp>
#include <boost/javascript/literal_directive.hpp>
#include <boost/javascript/parse_guard.hpp>

using namespace boost::javascript;

void javascript_grammar::definition<javascript_grammar::scanner_t>::define_grammar3()
{
    while_statement
        = root_node_d[literal_d[str_p("while")]] >> inner_node_d[parse_guard_d[ch_p('(')] >> parse_guard_d(expression,"expression") >> parse_guard_d[ch_p(')')]] >> parse_guard_d(statement,"statement");
    for_condition_standard
        = (
            (no_node_d[literal_d[str_p("var")]] >> variable_declaration_list_no_in >> no_node_d[ch_p(';')]) |
            (expression_statement_no_in|empty_statement) 
          ) >> (expression_statement|empty_statement) >> !expression
          ;
    for_statement
        = root_node_d[literal_d[str_p("for")]] >> inner_node_d[parse_guard_d[ch_p('(')] >> parse_guard_d[for_condition_standard | for_condition_in] >> parse_guard_d[ch_p(')')]] >> parse_guard_d(statement,"statement");
}
