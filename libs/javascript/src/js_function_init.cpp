#include <boost/javascript/js_function.hpp>
#include <boost/clipp/class.hpp>

using namespace boost::javascript;
using namespace boost::clipp;

void js_function::init(context* c)
{
    class_<js_function,object> cls("Function",c);
    cls.constructor(arguments<std::valarray<std::string>&>());
    cls.function("toString",&js_function::toString);
}
