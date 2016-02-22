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
    (void)c;

    //Parse a file
    double a=3.14;
    parser.parse("var a=3.14;");

    double b=12.14;
    parser.parse("var b=12.14;");

    int ia=34;
    parser.parse("var ia=34;");

    int ib=13;
    parser.parse("var ib=13;");

    assert(a+b == unwrap<double>(parser.parse("a+b;"))());
    assert(a-b == unwrap<double>(parser.parse("a-b;"))());
    assert(a*b == unwrap<double>(parser.parse("a*b;"))());
    assert(a/b == unwrap<double>(parser.parse("a/b;"))());
    assert(a == unwrap<double>(parser.parse("a%b;"))());
    assert((ia&ib) == unwrap<int>(parser.parse("ia&ib;"))());
    assert((ia|ib) == unwrap<int>(parser.parse("ia|ib;"))());
    assert((ia^ib) == unwrap<int>(parser.parse("ia^ib;"))());
    assert((ia<<10) == unwrap<int>(parser.parse("ia<<10;"))());
    assert((0x10>>1) == unwrap<int>(parser.parse("0x10>>1;"))());
    assert((0xa0000000>>1) == unwrap<unsigned>(parser.parse("0xa0000000>>>1;"))());
    assert(a++ == unwrap<double>(parser.parse("a++;"))());
    assert(++b == unwrap<double>(parser.parse("++b;"))());
    assert((a+=5) == unwrap<double>(parser.parse("a+=5;"))());
    assert((a-=2.3) == unwrap<double>(parser.parse("a-=2.3;"))());
    assert((a*=1.3) == unwrap<double>(parser.parse("a*=1.3;"))());
    assert(--a == unwrap<double>(parser.parse("--a;"))());
    assert((ia%=4) == unwrap<double>(parser.parse("ia%=4;"))());
    assert(b-- == unwrap<double>(parser.parse("b--;"))());
    assert((ib&=13) == unwrap<double>(parser.parse("ib&=13;"))());
    assert((ib|=234) == unwrap<int>(parser.parse("ib|=234;"))());
    assert((ib^=1000) == unwrap<int>(parser.parse("ib^=1000;"))());
}

