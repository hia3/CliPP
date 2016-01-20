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

    unwrap<double> val(parser.parse("eval('10*5+Math.PI;');"));
    assert(val.ok());
    double c_val=10*5+unwrap<double>(c->global()["Math"]["PI"])();
    //GCC returns false for the expression below. This is clearly wrong, as val()==c_val returns true...
    //assert(val()==10*5+unwrap<double>(c->global()["Math"]["PI"])());
    assert(val()==c_val);
    
    assert(unwrap<int>(parser.parse("parseInt('  -52',10);"))()==-52);
    assert(unwrap<int>(parser.parse("parseInt('0x4F');"))()==0x4f);
    assert(unwrap<int>(parser.parse("parseInt('056');"))()==056);
    assert(unwrap<double>(parser.parse("parseFloat('-3.5243');"))()==-3.5243);
    return 0;
}



/*void do_assert(int a,bool success)
{
    if(!success) throw exception("Failure!");
}

    function(c,"assert",do_assert);

//    std::ifstream file("../timer_test.js");
    try {
       parser.parse("var a='Hei!!\\n 86,42 \\t \\xa2 ds';");
       const char* text=unwrap<const char*>(parser.global()["a"])();
       std::cout << text;
       int a=0;
    }
    catch(exception& e){
        std::cout << e.what();
    }
    */
