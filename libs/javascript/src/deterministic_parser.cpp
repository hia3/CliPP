#include <boost/javascript/javascript_grammar.hpp>

//#define compound_case_parser ccp
//#include <boost/spirit/dynamic/switch.hpp>
#include <boost/spirit/error_handling.hpp>
#include <boost/javascript/literal_directive.hpp>

using namespace boost::javascript;

typedef assertion<const char*> my_assertion;
static my_assertion function_declaration_error("Unexpected function declaration");

void javascript_grammar::definition<javascript_grammar::scanner_t>::define_detgram()
{
    statement = 
        block |
        variable_statement |
        empty_statement |
        if_statement |
        do_while_statement |
        while_statement |
        for_statement |
        return_statement |
        continue_statement |
        break_statement |
        with_statement |
        switch_statement |
        throw_statement |
        try_statement |
        (literal_d[str_p("function")] >> function_declaration_error(nothing_p)) |
        expression_statement;
/*
    statement = switch_p2(access_node_d[statement_keywords][set_root_node_id()])
        [
            case_p<blockID>(statement_list >> discard_node_d[ch_p('}')]),
            case_p<variable_statementID>(discard_last_node_d[variable_declaration_list >> ';']),
            case_p<empty_statementID>(discard_node_d[epsilon_p]),
            case_p<if_statementID>(inner_node_d['(' >> expression >> ')'] >> statement >> !discard_first_node_d[(str_p("else") >> statement)]),
            case_p<do_while_statementID>(statement >> inner_node_d["while" >> inner_node_d[ch_p('(') >> expression >> ch_p(')')] >> ';']),
            case_p<while_statementID>(inner_node_d['(' >> expression >> ')'] >> statement),
            case_p<for_statementID>(inner_node_d['(' >> (for_condition_standard | for_condition_in) >> ')'] >> statement),
            case_p<return_statementID>(!expression >> discard_node_d[ch_p(';')]),
            case_p<continue_statementID>(discard_node_d[ch_p(';')]),
            case_p<break_statementID>(discard_node_d[ch_p(';')]),
            case_p<with_statementID>(inner_node_d['(' >> expression >> ')'] >> statement),
            case_p<switch_statementID>(
                inner_node_d['(' >> expression >> ')'] >> 
                    no_node_d[ch_p('{')] >> 
                        *case_clause >> 
                        !default_clause >>
                        *case_clause >>
                    no_node_d[ch_p('}')]
            ),
            case_p<throw_statementID>(expression_statement),
            case_p<try_statementID>(inner_node_d["{">> statement_list >> "}"] >> (catch_clause || finally_clause)),
            case_p<function_declarationID>(function_declaration_error(nothing_p)),
            default_p(expression_statement)
        ];
    statement_keywords.add
        ("{",blockID)
        ("var",variable_statementID)
        (";",empty_statementID)
        ("if",if_statementID)
        ("do",do_while_statementID)
        ("while",while_statementID)
        ("for",for_statementID)
        ("return",return_statementID)
        ("continue",continue_statementID)
        ("break",break_statementID)
        ("with",with_statementID)
        ("switch",switch_statementID)
        ("throw",throw_statementID)
        ("function",function_declarationID)
        ("try",try_statementID);*/
}

