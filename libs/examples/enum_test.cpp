#include <boost/clipp.hpp>
#include <boost/javascript.hpp>
#include <boost/clipp/clipp_stream.hpp>
#include <boost/javascript/date.hpp>
#include <strstream>
#include <iostream>
#include <conio.h>

using namespace boost::clipp;
using namespace boost::javascript;

enum color {
    red,
    green,
    blue
};

color get_blue() {
    return blue;
}

void test_color(color c) {
    assert(c==blue);
}

int main() 
{
    //Create a javascript parser
    javascript_parser parser;
    context* c=parser.get_context();
    enum_<color> cls("color",c);
    cls.value("red",red);
    cls.value("green",green);
    cls.global_value("blue",blue);

    function(c,"test_color",test_color);
    function(c,"get_blue",get_blue);

    unwrap<color> val(parser.parse(
        "var a=color.blue;\n"
        "test_color(a);\n"
        "test_color(get_blue());\n"
        "color.red;"
        ));
    assert(val.ok());
    assert(val()==red);
    return 0;
}
