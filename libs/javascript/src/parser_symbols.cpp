#include <boost/javascript/javascript_grammar.hpp>
#include <boost/javascript/literal_directive.hpp>
#include <boost/javascript/error_handler.hpp>

using namespace boost::javascript;

void javascript_grammar::definition<javascript_grammar::scanner_t>::define_symbols()
{
    // javascript keywords
    keywords 
        = "abstract" 
        , "boolean" 
        , "break" 
        , "byte" 
        , "case" 
        , "catch" 
        , "char" 
        , "class" 
        , "const" 
        , "continue" 
        , "debugger" 
        , "default" 
        , "delete" 
        , "do" 
        , "double" 
        , "else" 
        , "enum" 
        , "export" 
        , "extends" 
        , "false" 
        , "final" 
        , "finally" 
        , "float" 
        , "for" 
        , "function" 
        , "goto" 
        , "if" 
        , "implements" 
        , "import" 
        , "in" 
        , "instanceof" 
        , "int" 
        , "interface" 
        , "long" 
        , "native" 
        , "new" 
        , "null" 
        , "package" 
        , "private" 
        , "protected" 
        , "public" 
        , "return" 
        , "short" 
        , "static" 
        , "struct" 
        , "super" 
        , "switch" 
        , "synchronized" 
        , "this" 
        , "throw" 
        , "throws" 
        , "transient" 
        , "true" 
        , "try" 
        , "typeof" 
        , "var" 
        , "void" 
        , "volatile" 
        , "while" 
        , "with"
        ;

    multiplicative_operators 
        = "*"
        , "/"
        , "%"
        ;

    additive_operators
        = "+" 
        , "-"
        ;

    bitwise_shift_operators 
        = "<<"
        , ">>"
        , ">>>"
        ;

    relational_operators 
        = "<"
        , ">"
        , "<="
        , ">="
        , "instanceof"
        , "in"
        ;

    relational_operators_no_in 
        = "<"
        , ">"
        , "<="
        , ">="
        , "instanceof"
        ;

    equality_operators
        = "=="
        , "!="
        , "==="
        , "!=="
        ;
    assignment_operators
        = "="
        , "*="
        , "/="
        , "%="
        , "+="
        , "-="
        , "<<="
        , ">>="
        , ">>>="
        , "&="
        , "^="
        , "|="
        ;
    numeric_literal 
        =   lexeme_d[discard_node_d[ch_p('0') >> ch_p('x')] >> leaf_node_d[hex_p]] 
        |  lexeme_d[leaf_node_d[real_p]]
        ;
    function_declaration 
        = root_node_d[literal_d[str_p("function")]] >> assert_literal(identifier) >> formal_parameter_list >> function_body;

}

