#include <boost/clipp.hpp>
#include <boost/javascript.hpp>
#include <boost/clipp/clipp_stream.hpp>
#include <boost/javascript/date.hpp>
#include <strstream>
#include <iostream>
#include <conio.h>
#include <boost/javascript/error_handler.hpp>

using namespace boost::clipp;
using namespace boost::javascript;

//Define a simple javascript function

int main() 
{
    //Create a javascript parser
    javascript_parser parser;
    context* c=parser.get_context();

    //Parse a user defined function to calculate faculty of a number
    try {
    parser.parse(
        "function faculty(input) {\n"
        "   if(input<=1) return 1;\n"
        "   return input*faculty(input-1);\n"
        "}");
    }
    catch(boost::spirit::parser_error<javascript_error_handler>& e) {
        std::cout << e.descriptor.error_message;
    }
    catch(std::exception& e2) {
        std::cout << e2.what();
    }
    catch(...) {
        int a=0;
    }
    //Call the function with argument 10
    //Assert that the output is correct
    assert(3628800==unwrap<int>(c->global()["faculty"](10))());

    parser.parse("var f = function foo( input ){ return 10*input; };");
    std::cout << unwrap<std::string>(c->global()["f"]["toString"]())();
    parser.parse("var f2 = new function foo( input ){ return 20*input; };");
    std::cout << unwrap<std::string>(c->global()["f2"]["toString"]())();
    parser.parse("function SWAP(adata, j, i)"
                 "{"
                 "   var st=adata[j];"
                 "   adata[j] = adata[i];"
                 "   adata[i] = st;"
                 "}");
    parser.parse(
        "var cube=function(input) {\n"
        "   return Math.pow(input,3);\n"
        "};");
    assert(35.937==unwrap<double>(c->global()["cube"](3.3))());
}