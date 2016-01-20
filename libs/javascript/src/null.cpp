#include <boost/javascript/null.hpp>
#include <boost/clipp/class.hpp>

using namespace boost::clipp;
using namespace boost::javascript;

void null::init(context* c)
{
    if(is_type_defined<null>(c)) return;
    class_<null,object> cls("null",c);
    cls[self == self];
    cls[self == value()];
}
