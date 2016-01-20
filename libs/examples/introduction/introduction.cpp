#include <boost/clipp.hpp>
#include <boost/javascript.hpp>

using namespace boost::clipp;
using namespace boost::javascript;

//Define a simple javascript function
double square(const double& x)
{
    return x * x;
}

void main() 
{
    //Create a javascript parser
    javascript_parser parser;
    context* c=parser.get_context();

    //This line exposes the square function to the javascript parser.
    function(c,"square",square);

    //Calculate result of square(PI) and store it in a;
    parser.parse("var a=9*square(Math.PI);");


    //Extract the value from the Javascript context
    valueP result = parser.parse("a;");

    //Convert the value to a double
    double numerical = unwrap<double>(result)();

    //Call a javascript function on the value to convert it to a hexadecimal string.
    valueP result_text = result["toString"](16);

    //Convert the result to a string.
    std::string text=unwrap<std::string>(result_text)();
}
