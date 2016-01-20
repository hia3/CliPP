#include <boost/javascript/javascript_grammar.hpp>
#include <boost/javascript/literal_directive.hpp>
#include <boost/javascript/parse_guard.hpp>

using namespace boost::javascript;

void javascript_grammar::definition<javascript_grammar::scanner_t>::define_grammar8()
{
    for_condition_in
        = (
            (discard_node_d[literal_d[str_p("var")]] >> variable_declaration_no_in >> discard_node_d[literal_d[str_p("in")]] >> parse_guard_d[expression]) |
            (left_hand_side_expression >> discard_node_d[literal_d[str_p("in")]] >> parse_guard_d[expression]) 
          );
          ;
/*    logical_AND_expression_no_in
        = bitwise_OR_expression_no_in >>
        *(root_node_d[str_p("&&")] >> bitwise_OR_expression_no_in)
        ;
    logical_OR_expression
        = logical_AND_expression >>
        *(root_node_d[str_p("||")] >> logical_AND_expression)
        ;
    logical_OR_expression_no_in
        = logical_AND_expression_no_in >>
        *(root_node_d[str_p("||")] >> logical_AND_expression_no_in)
        ;*/
    function_expression 
        = root_node_d[literal_d[str_p("function")]] >> !identifier >> formal_parameter_list >> function_body;
    null_literal 
        = leaf_node_d[literal_d[str_p("null")]]
        ;

    boolean_literal
        = leaf_node_d[literal_d[str_p("true")]]
        | leaf_node_d[literal_d[str_p("false")]]
        ;

}

