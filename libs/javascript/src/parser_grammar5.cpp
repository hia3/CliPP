#include <boost/javascript/javascript_grammar.hpp>
#include <boost/javascript/literal_directive.hpp>
#include <boost/javascript/no_eol_directive.hpp>

using namespace boost::javascript;

void javascript_grammar::definition<javascript_grammar::scanner_t>::define_grammar5()
{
/*    statement 
        =  block 
        | variable_statement 
        | empty_statement
        | if_statement
        | do_while_statement
        | while_statement
        | for_statement
        | (!str_p("function") >> expression_statement)
        | return_statement
        | continue_statement
        | break_statement
        | with_statement
        | switch_statement
        | throw_statement
        | try_statement

        ;*/

    this_literal
        = leaf_node_d[literal_d[str_p("this")]]
        ;
    optional_semicolon
        = ch_p(';') | eps_p('}') | end_p | no_eol_d[eol_p];
}

