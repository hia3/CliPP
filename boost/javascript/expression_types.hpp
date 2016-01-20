// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_JAVASCRIPT_EXPRESSION_TYPES_HPP_HOLT_2003_0103
#define BOOST_JAVASCRIPT_EXPRESSION_TYPES_HPP_HOLT_2003_0103

#include "boost/javascript/config.hpp"

namespace boost { namespace javascript {

enum {
    identifierID=1,
    string_literalID,
    escape_sequenceID,
    hex_escape_sequenceID,
    character_escape_sequenceID,
    null_escape_sequenceID,
    string_charactersID,
    numeric_literalID,
    null_literalID,
    boolean_literalID,
    this_literalID,
    array_literalID,
    postfix_expressionID,
    unary_expressionID,
    multiplicative_expressionID,
    additive_expressionID,
    bitwise_shift_expressionID,
    relational_expressionID,
    relational_expression_no_inID,
    equality_expressionID,
    equality_expression_no_inID,
    bitwise_AND_expressionID,
    bitwise_AND_expression_no_inID,
    bitwise_XOR_expressionID,
    bitwise_XOR_expression_no_inID,
    bitwise_OR_expressionID,
    bitwise_OR_expression_no_inID,
    logical_AND_expressionID,
    logical_AND_expression_no_inID,
    logical_OR_expressionID,
    logical_OR_expression_no_inID,
    conditional_expressionID,
    conditional_expression_no_inID,
    assignment_expressionID,
    assignment_expression_no_inID,
    literalID,
    primary_expressionID,
    programID,
    function_expressionID,
    left_hand_side_expressionID,
    member_expressionID,
    new_expressionID,
    call_expressionID,
    argumentsID,
    variable_statementID,
    variable_declarationID,
    variable_declaration_no_inID,
    variable_declaration_listID,
    variable_declaration_list_no_inID,
    empty_statementID,
    expression_statementID,
    expression_statement_no_inID,
    if_statementID,
    do_while_statementID,
    while_statementID,
    for_condition_standardID,
    for_condition_inID,
    for_statementID,
    statementID,
    blockID,
    function_declarationID,
    formal_parameter_listID,
    function_bodyID,
    source_elementID,
    return_statementID,
    continue_statementID,
    break_statementID,
    with_statementID,
    switch_statementID,
    case_clauseID,
    default_clauseID,
    statement_listID,
    throw_statementID,
    try_statementID,
    catch_clauseID,
    finally_clauseID,
};

}} //namespace boost::javascript

#endif

