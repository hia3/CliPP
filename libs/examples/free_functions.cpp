#include <boost/clipp.hpp>
#include <boost/javascript.hpp>
#include <boost/clipp/clipp_stream.hpp>
#include <boost/javascript/date.hpp>
#include <strstream>
#include <iostream>
#include <conio.h>

using namespace boost::clipp;
using namespace boost::javascript;

static int a;
int get_a() {return a;}
void set_a(int val) {a=val;}


int main() 
{
    //Create a javascript parser
    javascript_parser parser;
    context* c=parser.get_context();
    read_write(c,"saveTrace",boost::ref(a),boost::ref(a));

    parser.parse("var scriptError;\n"
            "function initialize (timeStamp)\n"
            "{\n"
            "   saveTrace = 5;\n"
            "   scriptError = false;\n"
            "}\n");

    parser.parse("initialize(5);");


    unwrap<bool> val(parser.parse("scriptError;"));
    assert(val.ok());
    assert(val()==false);
    return 0;
}