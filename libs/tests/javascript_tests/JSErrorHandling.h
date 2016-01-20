#pragma once
#include <cppunit/extensions/HelperMacros.h>

class JSErrorHandling : public CPPUNIT_NS::TestFixture
{
public:
    CPPUNIT_TEST_SUITE( JSErrorHandling );
    CPPUNIT_TEST(ErrorParserPos);
    CPPUNIT_TEST(SyntaxErrorReservedWord);
    CPPUNIT_TEST(SyntaxErrorInvalidSymbolAtEndOfFunction);
    CPPUNIT_TEST(SyntaxErrorInvalidSymbolInFunction);
    CPPUNIT_TEST_SUITE_END();
protected:
    void ErrorParserPos();
    void SyntaxErrorReservedWord();
    void SyntaxErrorInvalidSymbolAtEndOfFunction();
    void SyntaxErrorInvalidSymbolInFunction();
};
