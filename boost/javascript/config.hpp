// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_JAVASCRIPT_CONFIG_HPP_15092003
#   define BOOST_JAVASCRIPT_CONFIG_HPP_15092003

#   ifndef BOOST_CONFIG_HPP
#       include "boost/config.hpp"
#   endif

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#define BOOST_WIN32
#endif


#   ifdef BOOST_MSVC 
//non-DLL-interface classkey 'identifier' used as base for DLL-interface classkey 'identifier'
#       pragma warning (disable:4275)
//'identifier' : identifier was truncated to 'number' characters in the debug information
#       pragma warning (disable:4786)
//'type' : forcing value to bool 'true' or 'false' (performance warning)
#       pragma warning (disable:4800)
//'identifier' : class 'type' needs to have dll-interface to be used by clients of class 'type2'
#       pragma warning (disable:4251) 
#   endif

#   ifdef BOOST_JAVASCRIPT_EXPORTS
#       define BOOST_JAVASCRIPT_EXPORT_IMPORT __declspec(dllexport)
#   else
#       define BOOST_JAVASCRIPT_EXPORT_IMPORT __declspec(dllimport)
#       ifdef _DEBUG
#           define BOOST_JAVASCRIPT_LIB_NAME "boost_javascriptD.lib"
#       else
#           define BOOST_JAVASCRIPT_LIB_NAME "boost_javascriptR.lib"
#       endif
#       pragma comment(lib,BOOST_JAVASCRIPT_LIB_NAME)
#   endif
#endif // BOOST_JAVASCRIPT_CONFIG_HPP_15092003

