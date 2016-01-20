// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef JAVASCRIPT_JAVASCRIPT_GRAMMAR_HPP_HOLT_2003_0103
#define JAVASCRIPT_JAVASCRIPT_GRAMMAR_HPP_HOLT_2003_0103

#pragma warning (disable:4355)

#include "boost/javascript/expression_types.hpp"
#include "boost/spirit/core.hpp"
#include "boost/spirit/tree/ast.hpp"
#include "boost/spirit/symbols.hpp"
#include "boost/javascript/javascript_parser.hpp"
#include <boost/spirit/utility/confix.hpp>

#include <iostream>
#include <fstream>
#include <vector>

using namespace boost::spirit;

namespace boost { namespace javascript {

///////////////////////////////////////////////////////////////////////////////
// parsing helper function

struct set_root_node_id
{
    template <typename TreeT, typename IterT>
    void operator()(TreeT& t, IterT begin, IterT end) const
    {
        t.value.id(parser_id(size_t(t.value.value())));
        t.value.is_root(true);
    }
};

//  Here's our comment rule
struct skip_grammar : public grammar<skip_grammar>
{
    template <typename ScannerT>
    struct definition
    {
        definition(skip_grammar const& /*self*/)
        {
#if defined(BOOST_MSVC) && (BOOST_MSVC <= 1200)

            skip=   space_p
                |   "//" >> *(anychar_p - '\n') >> '\n'     // C++ comment
                |   "/*" >> *(anychar_p - "*/") >> "*/"     // C comment
                ;
#else
            skip
                =   space_p
                |   comment_p("//")                 // C++ comment
                |   comment_p("/*", "*/")           // C comment
                ;
#endif
        }

        rule<ScannerT> skip;

        rule<ScannerT> const&
        start() const { return skip; }
    };
};

struct javascript_grammar : public grammar<javascript_grammar>
{
    typedef skip_parser_iteration_policy<skip_grammar> iter_policy_t;
    typedef ast_match_policy<javascript_parser::iterator_t, javascript_parser::factory_t> ast_match_policy_t;
    typedef
        scanner_policies<iter_policy_t, ast_match_policy_t>
        scanner_policies_t;
    typedef scanner<javascript_parser::iterator_t, scanner_policies_t> scanner_t;

    template <typename ScannerT>
    struct definition
    {
        definition(javascript_grammar const& /*self*/)
        {
        };
    };
};
    template<>
    struct javascript_grammar::definition<javascript_grammar::scanner_t>
    {
        typedef javascript_grammar::scanner_t scanner_t;
        void define_literals();
        void define_symbols();
        void define_expressions();
        void define_grammar1();
        void define_grammar2();
        void define_grammar3();
        void define_grammar4();
        void define_grammar5();
        void define_grammar6();
        void define_grammar7();
        void define_grammar8();
        void define_grammar9();
        void define_detgram();
        void define_statements();

        definition(javascript_grammar const& /*self*/)
        {
            define_symbols();
            define_literals();
            define_expressions();
            define_grammar1();
            define_grammar2();
            define_grammar3();
            define_grammar4();
            define_grammar5();
            define_grammar6();
            define_grammar7();
            define_grammar8();
            define_grammar9();
            define_statements();
            define_detgram();
        }
        symbols<> keywords;
        symbols<> postfix_operators;
        symbols<> unary_operators;
        symbols<> relational_operators;
        symbols<> relational_operators_no_in;
        symbols<> equality_operators;
        symbols<> multiplicative_operators;
        symbols<> additive_operators;
        symbols<> bitwise_shift_operators;
        symbols<> assignment_operators;
        symbols<> statement_keywords;
        symbols<> binary_operators;
        symbols<> binary_operators_no_in;
        symbols<> ternary_operator;

        rule<scanner_t, parser_context, parser_tag<identifierID> >                     identifier;
        rule<scanner_t, parser_context, parser_tag<string_literalID> >                 string_literal;
        rule<scanner_t, parser_context, parser_tag<numeric_literalID> >                numeric_literal;
        rule<scanner_t, parser_context, parser_tag<null_literalID> >                   null_literal;
        rule<scanner_t, parser_context, parser_tag<boolean_literalID> >                boolean_literal;
        rule<scanner_t, parser_context, parser_tag<literalID> >                        literal;
        rule<scanner_t, parser_context, parser_tag<array_literalID> >                  array_literal;        
        rule<scanner_t, parser_context, parser_tag<this_literalID> >                   this_literal;
        rule<scanner_t, parser_context, parser_tag<primary_expressionID> >             primary_expression;
        rule<scanner_t, parser_context, parser_tag<member_expressionID> >              member_expression;
        rule<scanner_t, parser_context, parser_tag<left_hand_side_expressionID> >      left_hand_side_expression;
//        rule<scanner_t, parser_context, parser_tag<postfix_expressionID> >             postfix_expression;
//        rule<scanner_t, parser_context, parser_tag<unary_expressionID> >               unary_expression;
/*        rule<scanner_t, parser_context, parser_tag<multiplicative_expressionID> >      multiplicative_expression;
        rule<scanner_t, parser_context, parser_tag<additive_expressionID> >            additive_expression;
        rule<scanner_t, parser_context, parser_tag<bitwise_shift_expressionID> >       bitwise_shift_expression;        
        rule<scanner_t, parser_context, parser_tag<relational_expressionID> >          relational_expression;        
        rule<scanner_t, parser_context, parser_tag<relational_expression_no_inID> >    relational_expression_no_in;        
        rule<scanner_t, parser_context, parser_tag<equality_expressionID> >            equality_expression;        
        rule<scanner_t, parser_context, parser_tag<equality_expression_no_inID> >      equality_expression_no_in;        
        rule<scanner_t, parser_context, parser_tag<bitwise_AND_expressionID> >         bitwise_AND_expression;
        rule<scanner_t, parser_context, parser_tag<bitwise_AND_expression_no_inID> >   bitwise_AND_expression_no_in;
        rule<scanner_t, parser_context, parser_tag<bitwise_XOR_expressionID> >         bitwise_XOR_expression;
        rule<scanner_t, parser_context, parser_tag<bitwise_XOR_expression_no_inID> >   bitwise_XOR_expression_no_in;
        rule<scanner_t, parser_context, parser_tag<bitwise_OR_expressionID> >          bitwise_OR_expression;
        rule<scanner_t, parser_context, parser_tag<bitwise_OR_expression_no_inID> >    bitwise_OR_expression_no_in;
        rule<scanner_t, parser_context, parser_tag<logical_AND_expressionID> >         logical_AND_expression;
        rule<scanner_t, parser_context, parser_tag<logical_AND_expression_no_inID> >   logical_AND_expression_no_in;*/
        rule<scanner_t, parser_context, parser_tag<logical_OR_expressionID> >          logical_OR_expression;
        rule<scanner_t, parser_context, parser_tag<logical_OR_expression_no_inID> >    logical_OR_expression_no_in;
        rule<scanner_t, parser_context, parser_tag<conditional_expressionID> >         conditional_expression;
        rule<scanner_t, parser_context, parser_tag<conditional_expression_no_inID> >   conditional_expression_no_in;
        rule<scanner_t, parser_context, parser_tag<assignment_expressionID> >          assignment_expression;
        rule<scanner_t, parser_context, parser_tag<assignment_expression_no_inID> >    assignment_expression_no_in;
        rule<scanner_t, parser_context, parser_tag<programID> >     program;
        rule<scanner_t, parser_context, parser_tag<new_expressionID> > new_expression;
        rule<scanner_t, parser_context, parser_tag<call_expressionID> > call_expression;
        rule<scanner_t, parser_context, parser_tag<argumentsID> > arguments;
        rule<scanner_t, parser_context, parser_tag<variable_statementID> > variable_statement;
        rule<scanner_t, parser_context, parser_tag<variable_declarationID> > variable_declaration;
        rule<scanner_t, parser_context, parser_tag<variable_declaration_no_inID> > variable_declaration_no_in;
        rule<scanner_t, parser_context, parser_tag<empty_statementID> > empty_statement;
        rule<scanner_t, parser_context, parser_tag<expression_statementID> > expression_statement;
        rule<scanner_t, parser_context, parser_tag<expression_statement_no_inID> > expression_statement_no_in;
        rule<scanner_t, parser_context, parser_tag<if_statementID> > if_statement;
        rule<scanner_t, parser_context, parser_tag<variable_declaration_listID> > variable_declaration_list;
        rule<scanner_t, parser_context, parser_tag<variable_declaration_list_no_inID> > variable_declaration_list_no_in;
        rule<scanner_t, parser_context, parser_tag<blockID> > block;
//        rule<scanner_t, parser_context, parser_tag<statementID> > statement;
        rule<scanner_t, parser_context, parser_tag<do_while_statementID> > do_while_statement;
        rule<scanner_t, parser_context, parser_tag<while_statementID> > while_statement;
        rule<scanner_t, parser_context, parser_tag<for_condition_standardID> > for_condition_standard;
        rule<scanner_t, parser_context, parser_tag<for_condition_inID> > for_condition_in;        
        rule<scanner_t, parser_context, parser_tag<for_statementID> > for_statement;
        rule<scanner_t, parser_context, parser_tag<function_declarationID> > function_declaration;
        rule<scanner_t, parser_context, parser_tag<function_expressionID> > function_expression;
        rule<scanner_t, parser_context, parser_tag<formal_parameter_listID> > formal_parameter_list;
        rule<scanner_t, parser_context, parser_tag<function_bodyID> > function_body;
        rule<scanner_t, parser_context, parser_tag<source_elementID> > source_element;
        rule<scanner_t, parser_context, parser_tag<return_statementID> > return_statement;
        rule<scanner_t, parser_context, parser_tag<continue_statementID> > continue_statement;
        rule<scanner_t, parser_context, parser_tag<break_statementID> > break_statement;
        rule<scanner_t, parser_context, parser_tag<with_statementID> > with_statement;
        rule<scanner_t, parser_context, parser_tag<switch_statementID> > switch_statement;
        rule<scanner_t, parser_context, parser_tag<case_clauseID> > case_clause;
        rule<scanner_t, parser_context, parser_tag<default_clauseID> > default_clause;
        rule<scanner_t, parser_context, parser_tag<statement_listID> > statement_list;
        rule<scanner_t, parser_context, parser_tag<throw_statementID> > throw_statement;
        rule<scanner_t, parser_context, parser_tag<try_statementID> > try_statement;
        rule<scanner_t, parser_context, parser_tag<catch_clauseID> > catch_clause;
        rule<scanner_t, parser_context, parser_tag<finally_clauseID> > finally_clause;
        
        subrule<0>  double_string_character;
        subrule<1>  single_string_character;
        subrule<2>  line_terminator;
        subrule<3>  escape_sequence;

        rule<scanner_t> expression,expression_no_in;
        rule<scanner_t> statement;
        rule<scanner_t> optional_semicolon;


        rule<scanner_t, parser_context, parser_tag<programID> > const&
        start() const { return program; }
    };
//};

}} //namespace boost::javascript

#endif

