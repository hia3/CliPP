#include <boost/clipp.hpp>
#include <boost/javascript.hpp>
#include <boost/clipp/clipp_stream.hpp>
#include <boost/javascript/date.hpp>
#include <strstream>
#include <iostream>
#include <conio.h>
#include <complex>

using namespace boost::clipp;
using namespace boost::javascript;

int main() 
{
    //Create a javascript parser
    javascript_parser parser;
    context* c=parser.get_context();


    parser.parse("var a=Math.i;\n"
                 "var b=Math.i;\n"
                 "a = a * b / 2 + b * 34;\n"
                 );
    parser.parse(
        "var a = Math.sqrt(-4);\n"
        "var b = a*Math.sin(a+15);\n"
        "var c = a+Math.sinh(Math.i*127/(5-Math.i));\n"
    );
    std::complex<double> i(0,1);
    std::complex<double> a = std::sqrt(std::complex<double>(-4,0));
    std::complex<double> b = a*std::sin(a+15.);
    std::complex<double> cc = a + std::sinh(i*127./(5.-i));

    assert(a == unwrap<std::complex<double> >(parser.global()["a"])());
    assert(cc == unwrap<std::complex<double> >(parser.global()["c"])());
    return 0;
}
