#include <boost/javascript/javascript_grammar.hpp>
#include <boost/javascript/literal_directive.hpp>

using namespace boost::javascript;

void javascript_grammar::definition<javascript_grammar::scanner_t>::define_grammar6()
{
    program = *source_element;
    formal_parameter_list
        = root_node_d[ch_p('(')] >> !(identifier >> *discard_first_node_d[',' >> identifier]) >> no_node_d[ch_p(')')];
    function_body
        = leaf_node_d[ch_p('{')] >> *(source_element) >> leaf_node_d[ch_p('}')];
    continue_statement
        = root_node_d[literal_d[str_p("continue")]] >> discard_node_d[optional_semicolon];
    break_statement
        = root_node_d[literal_d[str_p("break")]] >> discard_node_d[optional_semicolon];
    with_statement
        = root_node_d[literal_d[str_p("with")]] >> inner_node_d['(' >> expression >> ')'] >> statement;

    switch_statement
        = root_node_d[literal_d[str_p("switch")]] >> inner_node_d['(' >> expression >> ')'] >> 
          no_node_d[ch_p('{')] >> 
            *case_clause >> 
            !default_clause >>
            *case_clause >>
          no_node_d[ch_p('}')];

    case_clause
        = root_node_d[literal_d[str_p("case")]] >> expression >> no_node_d[ch_p(':')] >> *statement;
    default_clause
        = root_node_d[literal_d[str_p("default")]] >> no_node_d[ch_p(':')] >> *statement;
}

