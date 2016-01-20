# CliPP
CliPP is a C++ library which enables seamless interoperability between C++ and interpreted languages such as javascript. 
CliPP allows you to expose classes and functions written in C++ to Javascript with a minimum of extra code, and with a minimal impact on your existing code design.

The CliPP package also contains a complete Javascript Parser written using the Spirit library. The Javascript Parser is an independent part of the package, and can be replaced with another parser (not necessarily a javascript parser) if required.

A lot of the interface design, and many template-solutions inherits from the design in boost::python.

CliPP as a whole relies heavily on the Boost 1.32 library.

Originally developed by Peder Holt.

This is fork from http://clipp.sourceforge.net/

Requires C++14 compiler.
