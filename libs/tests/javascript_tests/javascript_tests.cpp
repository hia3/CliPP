#include <cppunit/plugin/TestPlugIn.h>
// Implements all the plug-in stuffs, WinMain...
CPPUNIT_PLUGIN_IMPLEMENT();

#ifdef _DEBUG
#pragma comment(lib, "cppunitd_dll.lib")
#else
#pragma comment(lib, "cppunit_dll.lib")
#endif
