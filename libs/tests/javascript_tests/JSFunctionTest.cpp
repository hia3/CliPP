#pragma warning (disable:4267)

#include "JSFunctionTest.h"
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

CPPUNIT_TEST_SUITE_REGISTRATION( JSFunctionTest );

void JSFunctionTest::FreeFunction()
{
    javascript_parser parser;
    parser.parse(
        "function foo()\n"
        "{\n"
        "}\n"
        "\n"
        "foo();\n"
        );
}

void JSFunctionTest::FreeFunctionWithArguments()
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
    CPPUNIT_ASSERT(val.ok());
    CPPUNIT_ASSERT_EQUAL(false,val());
    CPPUNIT_ASSERT_EQUAL(5,unwrap<int>(parser.parse("saveTrace"))());
}

class ErrorHandlerWithDebugging : public boost::javascript::cerr_handler {
public:
    ErrorHandlerWithDebugging() :m_count(0){}
    ~ErrorHandlerWithDebugging() {CPPUNIT_ASSERT_EQUAL(11u,m_count);}
    virtual void report_error(std::string const& message) const{
        CPPUNIT_ASSERT_MESSAGE(message,false);
    }
    virtual void branch_call() const
    {
        ++m_count;
    }
private:
    mutable size_t m_count;
};

void JSFunctionTest::FunctionWithReturn()
{
    //Create a javascript parser
    javascript_parser parser;
    context* c=parser.get_context();
    ErrorHandlerWithDebugging handler;

    parser.parse(        
        "function functionWithReturn()\n"
        "{\n"        
        "   for(var a=0;a<10;++a) {var b=3;}\n"
        "   return;\n"
        "}\n",handler);

    parser.parse("functionWithReturn();",handler);
}
