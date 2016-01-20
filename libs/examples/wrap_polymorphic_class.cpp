#include <boost/clipp.hpp>
#include <boost/javascript.hpp>
#include <conio.h>

using namespace boost::clipp;
using namespace boost::javascript;


class Base {
public:
    Base() {}
    Base(const Base& b) {}
    virtual ~Base() {}
};

class Derived : public Base {
public:
    Derived() {}
    Derived(const Derived& d) : Base(d) {}
    virtual ~Derived() {}
    std::string message() const {return "In Derived function";}
};

class DerivedWrapper : public object {
public:
    DerivedWrapper(const Derived& derived) : derived_(derived) {}
    DerivedWrapper() {}
    operator const Derived&() const {return derived_;}
    const Base& get_base() const {return derived_;}
    static void init(context* c) {
        class_<DerivedWrapper,object,Derived> cls("Derived",c);
        cls.constructor(arguments<>());
        cls.function("message",Derived::message);
        cls.read("base",get_base);
    }
private:
    Derived derived_;
};

//Define a simple javascript function
double square(const double& x)
{
    return x * x;
}

void main() 
{
    //Create a javascript parser
    javascript_parser parser;
    context* c=parser.get_context();
    DerivedWrapper::init(c);

    parser.parse("var derived=Derived();");
    parser.parse("var base=derived.base;");
    valueP result = parser.parse("base.message();");
    std::string outcome = unwrap<std::string>(result)();
}