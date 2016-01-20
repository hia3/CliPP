#include <boost/javascript/javascript_grammar.hpp>

using namespace boost::javascript;
struct string_grammar : public grammar<string_grammar> {
    template <typename ScannerT>
    struct definition
    {
        rule<ScannerT, parser_context, parser_tag<escape_sequenceID> > escape_sequence;
        rule<ScannerT, parser_context, parser_tag<hex_escape_sequenceID> > hex_escape_sequence;
        rule<ScannerT, parser_context, parser_tag<character_escape_sequenceID> > character_escape_sequence;
        rule<ScannerT, parser_context, parser_tag<null_escape_sequenceID> > null_escape_sequence;
        rule<ScannerT, parser_context, parser_tag<string_charactersID> > single_string_characters;
        rule<ScannerT, parser_context, parser_tag<string_charactersID> > double_string_characters;
        rule<ScannerT, parser_context, parser_tag<string_literalID> > string_literal;
        rule<ScannerT> line_terminator;

        definition(string_grammar const& /*self*/)
        {
            escape_sequence = no_node_d[ch_p('\\')] >> (
                hex_escape_sequence | 
                null_escape_sequence |
                character_escape_sequence
            );
            hex_escape_sequence = no_node_d[ch_p('x')] >> leaf_node_d[xdigit_p >> xdigit_p];
            null_escape_sequence = leaf_node_d[ch_p('0')] >> !(
                eps_p('0')|eps_p('1')|eps_p('2')|eps_p('3')|eps_p('4')|
                eps_p('5')|eps_p('6')|eps_p('7')|eps_p('8')|eps_p('9')
            );
            character_escape_sequence = leaf_node_d[anychar_p-line_terminator];
            line_terminator = ch_p(char(0x0a)) | ch_p(char(0x0d));
            single_string_characters = leaf_node_d[+(anychar_p-'\''-'\\'-line_terminator)];
            double_string_characters = leaf_node_d[+(anychar_p-'\"'-'\\'-line_terminator)];
            string_literal = inner_node_d[ch_p('\'') >> *(single_string_characters | escape_sequence) >> ch_p('\'')] |
                             inner_node_d[ch_p('\"') >> *(double_string_characters | escape_sequence) >> ch_p('\"')];
        };

        rule<ScannerT, parser_context, parser_tag<string_literalID> > const&
        start() const { return string_literal; }
    };
};
static string_grammar string_grammar_p;
void javascript_grammar::definition<javascript_grammar::scanner_t>::define_literals()
{

    string_literal = lexeme_d[string_grammar_p];
}
