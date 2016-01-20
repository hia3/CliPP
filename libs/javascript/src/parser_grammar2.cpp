#include <boost/javascript/javascript_grammar.hpp>
#include <boost/javascript/literal_directive.hpp>
#include <boost/javascript/parse_guard.hpp>

using namespace boost::javascript;

void javascript_grammar::definition<javascript_grammar::scanner_t>::define_grammar2()
{
    member_expression
        = 
        (
            (
                  primary_expression
                | function_expression
            ) >> 
            *(
                  (root_node_d[ch_p('[')] >> parse_guard_d[expression] >> parse_guard_d[no_node_d[ch_p(']')]])
                | (root_node_d[ch_p('.')] >> parse_guard_d[identifier])
            )
        )
        | root_node_d[literal_d[str_p("new")]] >> parse_guard_d[member_expression] >> arguments;
        ;

}
