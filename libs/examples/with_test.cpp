#include <boost/clipp.hpp>
#include <boost/javascript.hpp>
#include <boost/clipp/clipp_stream.hpp>
#include <boost/javascript/date.hpp>
#include <strstream>
#include <iostream>
#include <conio.h>

using namespace boost::clipp;
using namespace boost::javascript;

int main() 
{
    //Create a javascript parser
    javascript_parser parser;
    context* c=parser.get_context();

    parser.parse("var a=\"hei hopp\";");
    valueP result = parser.parse("with(a) charAt(1);");
    unwrap<char> text(result);
    assert(text.ok());
    assert(text()=='e');
}
