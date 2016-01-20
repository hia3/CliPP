#include <boost/clipp.hpp>
#include <boost/javascript.hpp>
#include <boost/clipp/clipp_stream.hpp>
#include <boost/javascript/date.hpp>
#include <strstream>
#include <iostream>
#include <conio.h>

using namespace boost::clipp;
using namespace boost::javascript;

void main() 
{
    //Create a javascript parser
    javascript_parser parser;
    context* c=parser.get_context();
    parser.parse(
        "var a='b';"
        "var b=0;"
        "switch(a) {"
        "  case 'a': b=0;"
        "  case 'b': b=1;"
        "  default : b=-1;"
        "}"
        );
    valueP result = parser.global()["b"];
    unwrap<int> val(result);
    assert(val.ok());
    assert(val()==1);
}
