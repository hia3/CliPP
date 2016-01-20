#include <boost/javascript/javascript_grammar.hpp>

using namespace boost::javascript;

void javascript_grammar::definition<javascript_grammar::scanner_t>::define_grammar4()
{
// javascript identifiers
    identifier = 
        leaf_node_d[ 
            lexeme_d[
                (
                    (alpha_p | '_' | '$') >> 
                    *(alnum_p | '_' | '$')
                ) - 
                (
                    keywords >> 
                    anychar_p - (alnum_p | '_' | '$')
                )
            ]
        ]
        ;
    /*    equality_expression
        = relational_expression >>
        *(root_node_d[equality_operators] >> relational_expression)
        ;

    equality_expression_no_in
        = relational_expression_no_in >>
        *(root_node_d[equality_operators] >> relational_expression_no_in)
        ;

    bitwise_AND_expression
        = equality_expression >>
        *(root_node_d[ch_p('&')] >> equality_expression)
        ;
    bitwise_AND_expression_no_in
        = equality_expression_no_in >>
        *(root_node_d[ch_p('&')] >> equality_expression_no_in)
        ;
    bitwise_XOR_expression
        = bitwise_AND_expression >>
        *(root_node_d[ch_p('^')] >> bitwise_AND_expression)
        ;
    bitwise_XOR_expression_no_in
        = bitwise_AND_expression_no_in >>
        *(root_node_d[ch_p('^')] >> bitwise_AND_expression_no_in)
        ;
    bitwise_OR_expression
        = bitwise_XOR_expression >>
        *(root_node_d[ch_p('|')] >> bitwise_XOR_expression)
        ;
    bitwise_OR_expression_no_in
        = bitwise_XOR_expression_no_in >>
        *(root_node_d[ch_p('|')] >> bitwise_XOR_expression_no_in)
        ;
    logical_AND_expression
        = bitwise_OR_expression >>
        *(root_node_d[str_p("&&")] >> bitwise_OR_expression)
        ;*/

}
