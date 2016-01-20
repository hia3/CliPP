#include <boost/javascript/javascript_grammar.hpp>
#include <boost/javascript/precedence.hpp>
#include <boost/javascript/literal_directive.hpp>
#include <boost/javascript/no_eol_directive.hpp>

using namespace boost::javascript;

void javascript_grammar::definition<javascript_grammar::scanner_t>::define_expressions()
{
/*    postfix_expression
        = left_hand_side_expression >> 
        !(
               root_node_d[str_p("++")]
            | root_node_d[str_p("--")]
        )
        ;

    unary_expression
        =  root_node_d[unary_operators] >> unary_expression
        | postfix_expression
        ;*/
    
/*    assignment_expression 
        = precedence_p(left_hand_side_expression)[
            postfix_p(root_node_d[postfix_operators]),
            unary_p(root_node_d[unary_operators]),
            binary_p(root_node_d[binary_operators]),
            binary_first_p(root_node_d[assignment_operators])
            ternary_p<conditional_expressionID>(root_node_d[ch_p('?')],no_node_d[ch_p(':')]);
          ];*/

    assignment_expression = precedence_p(left_hand_side_expression)
        [
            postfix_operator_p(root_node_d[no_eol_d[postfix_operators]]),
            unary_operator_p(root_node_d[literal_d[unary_operators]]),
            binary_operator_p(root_node_d[literal_d[binary_operators]]),
            ternary_operator_p(root_node_d[ternary_operator],assignment_expression >> no_node_d[ch_p(':')] >> assignment_expression),
            binary_assignment_operator_p(root_node_d[assignment_operators])
        ]
        ;
    assignment_expression_no_in = precedence_p(left_hand_side_expression)
        [
            postfix_operator_p(root_node_d[postfix_operators]),
            unary_operator_p(root_node_d[literal_d[unary_operators]]),
            binary_operator_p(root_node_d[literal_d[binary_operators_no_in]]),
            ternary_operator_p(root_node_d[ternary_operator],assignment_expression >> no_node_d[ch_p(':')] >> assignment_expression),
            binary_assignment_operator_p(root_node_d[assignment_operators])
        ]
        ;

    postfix_operators.add
        ("++",postfix_expressionID)
        ("--",postfix_expressionID)
        ;

    unary_operators.add
        ("delete",unary_expressionID)
        ("void",unary_expressionID)
        ("typeof",unary_expressionID)
        ("--",unary_expressionID)
        ("++",unary_expressionID)
        ("+",unary_expressionID)
        ("-",unary_expressionID)
        ("~",unary_expressionID)
        ("!",unary_expressionID)
        ;

    ternary_operator.add
        ("?",conditional_expressionID)
        ;
//        = precedence_p(unary_expression,root_node_d[binary_operators]);

//    logical_OR_expression_no_in = unary_expression;
//        = precedence_p(unary_expression,root_node_d[binary_operators_no_in]);

/*    assignment_expression
        = (left_hand_side_expression >> root_node_d[assignment_operators] >> assignment_expression)
        | conditional_expression
        ;
    assignment_expression_no_in
        = (left_hand_side_expression >> root_node_d[assignment_operators] >> assignment_expression_no_in)
        | conditional_expression_no_in
        ;
/*    conditional_expression
        = logical_OR_expression >>
        *(root_node_d[ch_p('?')] >> assignment_expression >> no_node_d[ch_p(':')] >> assignment_expression)
        ;

    conditional_expression_no_in
        = logical_OR_expression_no_in >>
        *(root_node_d[ch_p('?')] >> assignment_expression >> no_node_d[ch_p(':')] >> assignment_expression_no_in)
        ;*/

    assignment_operators.add
        ("*=",assignment_expressionID)
        ("/=",assignment_expressionID)
        ("%=",assignment_expressionID)
        ("+=",assignment_expressionID)
        ("-=",assignment_expressionID)
        ("<<=",assignment_expressionID)
        (">>=",assignment_expressionID)
        (">>>=",assignment_expressionID)
        ("&=",assignment_expressionID)
        ("^=",assignment_expressionID)
        ("|=",assignment_expressionID)
        ;

    binary_operators.add
        ("*",multiplicative_expressionID)
        ("/",multiplicative_expressionID)
        ("%",multiplicative_expressionID)
        ("+",additive_expressionID)
        ("-",additive_expressionID)
        ("<<",bitwise_shift_expressionID)
        (">>",bitwise_shift_expressionID)
        (">>>",bitwise_shift_expressionID)
        ("<",relational_expressionID)
        (">",relational_expressionID)
        ("<=",relational_expressionID)
        (">=",relational_expressionID)
        ("instanceof",relational_expressionID)
        ("in",relational_expressionID)
        ("==",equality_expressionID)
        ("!=",equality_expressionID)
        ("===",equality_expressionID)
        ("!==",equality_expressionID)
        ("&",bitwise_AND_expressionID)
        ("^",bitwise_XOR_expressionID)
        ("|",bitwise_OR_expressionID)
        ("&&",logical_AND_expressionID)
        ("||",logical_OR_expressionID)
/*        ("=",assignment_expressionID)
        ("*=",assignment_expressionID)
        ("/=",assignment_expressionID)
        ("%=",assignment_expressionID)
        ("+=",assignment_expressionID)
        ("-=",assignment_expressionID)
        ("<<=",assignment_expressionID)
        (">>=",assignment_expressionID)
        (">>>=",assignment_expressionID)
        ("&=",assignment_expressionID)
        ("^=",assignment_expressionID)
        ("|=",assignment_expressionID)*/;

    binary_operators_no_in.add
        ("*",multiplicative_expressionID)
        ("/",multiplicative_expressionID)
        ("%",multiplicative_expressionID)
        ("+",additive_expressionID)
        ("-",additive_expressionID)
        ("<<",bitwise_shift_expressionID)
        (">>",bitwise_shift_expressionID)
        (">>>",bitwise_shift_expressionID)
        ("<",relational_expressionID)
        (">",relational_expressionID)
        ("<=",relational_expressionID)
        (">=",relational_expressionID)
        ("instanceof",relational_expressionID)
        ("==",equality_expressionID)
        ("!=",equality_expressionID)
        ("===",equality_expressionID)
        ("!==",equality_expressionID)
        ("&",bitwise_AND_expressionID)
        ("^",bitwise_XOR_expressionID)
        ("|",bitwise_OR_expressionID)
        ("&&",logical_AND_expressionID)
        ("||",logical_OR_expressionID)
/*        ("=",assignment_expressionID)
        ("*=",assignment_expressionID)
        ("/=",assignment_expressionID)
        ("%=",assignment_expressionID)
        ("+=",assignment_expressionID)
        ("-=",assignment_expressionID)
        ("<<=",assignment_expressionID)
        (">>=",assignment_expressionID)
        (">>>=",assignment_expressionID)
        ("&=",assignment_expressionID)
        ("^=",assignment_expressionID)
        ("|=",assignment_expressionID)*/;
/*

    multiplicative_expression
        = unary_expression >>
        *(root_node_d[multiplicative_operators] >> unary_expression)
        ;


    additive_expression
        = multiplicative_expression >>
        *(root_node_d[additive_operators] >> multiplicative_expression)
        ;


    bitwise_shift_expression
        = additive_expression >>
        *(root_node_d[bitwise_shift_operators] >> additive_expression)
        ;


    relational_expression
        = bitwise_shift_expression >> 
        *(root_node_d[relational_operators] >> bitwise_shift_expression)
        ;


    relational_expression_no_in
        = bitwise_shift_expression >>
        *(root_node_d[relational_operators_no_in] >> bitwise_shift_expression)
        ;*/
}
