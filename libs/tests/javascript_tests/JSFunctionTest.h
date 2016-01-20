#pragma once
#include <cppunit/extensions/HelperMacros.h>

class JSFunctionTest : public CPPUNIT_NS::TestFixture
{
public:
    CPPUNIT_TEST_SUITE( JSFunctionTest );
    CPPUNIT_TEST(FreeFunction);
    CPPUNIT_TEST(FreeFunctionWithArguments);
    CPPUNIT_TEST(FunctionWithReturn);
    CPPUNIT_TEST_SUITE_END();
protected:
    void FreeFunction();
    void FreeFunctionWithArguments();
    void FunctionWithReturn();
};
