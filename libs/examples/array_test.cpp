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
    int line=0;
    javascript_parser parser;
    context* c=parser.get_context();

    parser.parse("var a = Array(1,3,2,4,5);");
    valueP a = c->global()["a"];    

    assert(unwrap<unsigned>(a["length"])()==5);
    assert(unwrap<int>(a["shift"]())()==1);
    assert(unwrap<unsigned>(a["length"])()==4);
    //VC7.0 crashes here.
#if !BOOST_WORKAROUND(BOOST_MSVC,==1300)
    a["sort"]();
    assert(unwrap<unsigned>(a["0"])()==2);
#endif
    return 0;
}
