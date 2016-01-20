#include <boost/clipp.hpp>
#include <boost/javascript.hpp>
#include <conio.h>
#include <boost/javascript/reference.hpp>

using namespace boost::clipp;
using namespace boost::javascript;

int main() 
{
    //Create a javascript parser
    javascript_parser parser;
    context* c=parser.get_context();

    //Test for(a;b;c)
    parser.parse(
        "var b=0;"
        "for(a=0;a<10;++a) {"
        "   if(a<2) continue;"
        "   b+=a;           "         
        "   if(a>5) break;  "
        "}");
    assert(unwrap<int>(c->global()["b"])() == 2+3+4+5+6);

    //Test for(a in b)
    parser.parse(
        "var a=new Array();"
        "a[0]=3.14;"
        "a[1]=58.67;"
        "a[2]=10;"
        "var c=0;"
        "for(var b in a) {"
        "   c+=b;"
        "}");
    assert(unwrap<double>(c->global()["c"])()==3.14+58.67+10);

    //test do while
    parser.parse(
        "var i=0;"
        "var c=0;"
        "do {"
        "   c+=a[i++];"
        "}while(i<a.length);");            
    assert(unwrap<double>(c->global()["c"])()==3.14+58.67+10);

    //test while parser
    parser.parse(
        "var i=0;"
        "var c=0;"
        "while(i<a.length) {"
        "   c+=a[i++];"
        "}");
    assert(unwrap<double>(c->global()["c"])()==3.14+58.67+10);
    return 0;
}
