#include <boost/javascript/javascript_grammar.hpp>
#include <boost/javascript/literal_directive.hpp>
#include <boost/javascript/parse_guard.hpp>

using namespace boost::javascript;

void javascript_grammar::definition<javascript_grammar::scanner_t>::define_grammar1()
{
    primary_expression
        = this_literal
        | literal
        | identifier
        ;

    arguments
        = 
        root_node_d[ch_p('(')] >> 
        !(
            expression >> *(no_node_d[ch_p(',')] >> parse_guard_d[expression])
        ) >>
        parse_guard_d[no_node_d[ch_p(')')]];
        ;

    new_expression 
        =
        (
              member_expression
            | root_node_d[literal_d[str_p("new")]] >> parse_guard_d[new_expression]
        )
        ;

    call_expression
        =   member_expression >> arguments >>
        *(
            (root_node_d[ch_p('[')] >> parse_guard_d(expression,"expression") >> parse_guard_d[no_node_d[ch_p(']')]])
           |(root_node_d[ch_p('.')] >> parse_guard_d[identifier])
           |arguments
        )
        ;

    left_hand_side_expression 
        = call_expression 
        | new_expression
        ;
    
    expression = assignment_expression;
    expression_no_in = assignment_expression_no_in;
}
