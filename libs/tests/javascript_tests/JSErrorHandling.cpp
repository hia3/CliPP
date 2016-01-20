#include "JSErrorHandling.h"
#include <boost/clipp.hpp>
#include <boost/javascript.hpp>
#include <boost/clipp/clipp_stream.hpp>
#include <boost/javascript/date.hpp>
#include <boost/javascript/message_handler.hpp>
#include <strstream>
#include <iostream>
#include <conio.h>

using namespace boost::clipp;
using namespace boost::javascript;

CPPUNIT_TEST_SUITE_REGISTRATION( JSErrorHandling );
class MyErrorHandler : public boost::javascript::cerr_handler {
public:
    MyErrorHandler(int errorLine) : m_errorLine(errorLine) , m_handled(false) {}
    ~MyErrorHandler() {
        CPPUNIT_ASSERT(m_handled);
    }
    virtual void report_error(std::string const& message) const{
        std::cout << message;
        CPPUNIT_ASSERT_EQUAL(m_errorLine,parser_pos().get_current_line());
        m_handled=true;
    }
private:
    int m_errorLine;
    mutable bool m_handled;
};

void test() {
    throw std::runtime_error("Error");
}

void JSErrorHandling::ErrorParserPos()
{
    MyErrorHandler handler(3);
    javascript_parser parser;
    context* c=parser.get_context();
    function(c,"test",&test);

    parser.parse(
        "var a=4;\n"
        "var b=5.6;\n"
        "var c=test();\n",
        handler
        );
}

class System : public object 
{
public:
    System() {}
    void printf(const std::string& txt) {};
    static void init(context* c) {
        class_<System> cls("System",c);
        cls[constructor<>()];
        cls.function("printf",&System::printf);
    }
};

void JSErrorHandling::SyntaxErrorReservedWord()
{
    MyErrorHandler handler(2);
    javascript_parser parser;
    System::init(parser.get_context());

    parser.parse(
        "var s = new System();\n"
        "functions foo()\n"
        "{\n"
        "   s.printf(\"Hello world\\n\");\n"
        "}\n"
        "foo();",
        handler
        );
}

void JSErrorHandling::SyntaxErrorInvalidSymbolAtEndOfFunction()
{
    MyErrorHandler handler(5);
    javascript_parser parser;
    System::init(parser.get_context());

    parser.parse(
        "var s = new System();\n"
        "function foo()\n"
        "{\n"        
        "   s.printf(\"Hello world\\n\");\n"
        "}]\n"
        "foo();",
        handler
        );
}

void JSErrorHandling::SyntaxErrorInvalidSymbolInFunction()
{
    MyErrorHandler handler(2);
    javascript_parser parser;
    System::init(parser.get_context());

    parser.parse(
        "var s = new System();\n"
        "function foo()\n"
        "{\n"
        "   s.printf('Hello world\\n');]\n"
        "}\n"
        "foo();",
        handler
        );
}

