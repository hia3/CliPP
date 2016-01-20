#include <boost/clipp/container_wrapper.hpp>
#include <boost/clipp/class.hpp>

using namespace boost::clipp;

void container_base::init(context* c)
{
    if(is_type_defined<container_base>(c)) return;
    class_<container_base,object> cls("container_base",c);
}
