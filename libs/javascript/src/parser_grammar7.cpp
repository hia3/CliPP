#include <boost/javascript/javascript_grammar.hpp>
#include <boost/javascript/literal_directive.hpp>
#include <boost/javascript/no_eol_directive.hpp>
#include <boost/javascript/parse_guard.hpp>

using namespace boost::javascript;

void javascript_grammar::definition<javascript_grammar::scanner_t>::define_grammar7()
{
    array_literal
        = root_node_d[ch_p('[')] >> 
            *((discard_node_d[!ch_p(',')] >> assignment_expression) | leaf_node_d[ch_p(',')]) >>
          discard_node_d[ch_p(']')]
        ;
    throw_statement
        = root_node_d[literal_d[str_p("throw")]] >> eps_p(no_eol_d[anychar_p - eol_p]) >> expression_statement;
    try_statement
        = root_node_d[literal_d[str_p("try")]] >> parse_guard_d[block] >> (catch_clause || finally_clause);
    catch_clause
        = root_node_d[literal_d[str_p("catch")]] >> inner_node_d['(' >> expression >> ')'] >> block;
    finally_clause
        = root_node_d[literal_d[str_p("finally")]] >> parse_guard_d[block];

}

