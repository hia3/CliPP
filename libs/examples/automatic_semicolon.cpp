#include <boost/clipp.hpp>
#include <boost/javascript.hpp>
#include <boost/clipp/clipp_stream.hpp>
#include <boost/javascript/date.hpp>
#include <strstream>
#include <iostream>
#include <conio.h>

using namespace boost::clipp;
using namespace boost::javascript;

void alert(const std::string& name)
{
    std::cout << name << std::endl;
}

int main() 
{
    //Create a javascript parser
    javascript_parser parser;
    context* c=parser.get_context();
    std::cout << "Expected Failure:\n";
    //Supposed to fail
    parser.parse("{ 1 2 } 3");

    std::cout << "Expected Success:\n";
    //Supposed to succeed
    parser.parse("{ 1\n2 } 3");

    std::cout << "Expected Failure:\n";
    //Supposed to fail
    parser.parse("for(a;b\n)");

    std::cout << "Expected Success:\n";
    //Supposed to succeed
    parser.parse("return\na+b");

    std::cout << "Expected Success:\n";
    //Supposed to succeed
    parser.parse("a = b\n++c");

    std::cout << "Expected Failure:\n";
    //Supposed to fail
    parser.parse("if (a > b)\nelse c=d");

    std::cout << "Expected Success:\n";
    //Supposed to succeed, but c(d+e) is called as a function
    parser.parse("a = b + c\n(d+e).print()");
    
    function(c,"alert",alert);
    //This bug
    parser.parse("var\n"
                 "gDemo = new Array();\n"
                 "var\n"
                 "name_ = 'noname';\n"
                 "function\n"
                 "MyObj()\n"
                 "{\n"
                 "this.name_ = 'name';\n"
                 "alert(this.name_);\n"
                 "}\n"
                 "var\n"
                 "myObj = new MyObj();\n"
                 "alert(name_);\n");
    parser.parse("var gDemo;\n"
                 "function demo()\n"
                 "{\n"
                 "   gDemo = 1;\n"
                 "}\n"
                 "demo();\n");

    parser.parse("var b=5;\n"
                 "obj=Object();\n"
                 "function a() {this.b=3;}\n"
                 "obj.fn=a;\n"
                 "obj.fn();\n");
    int result=unwrap<int>(parser.parse("obj.b"))();

    return 0;
}