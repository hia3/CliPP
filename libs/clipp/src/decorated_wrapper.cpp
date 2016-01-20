#include <boost/clipp/detail/decorated_wrapper.hpp>
#include <boost/clipp/detail/wrapper.hpp>
#include <boost/clipp/context.hpp>
using namespace boost::clipp;

void detail::decorated_wrapper::add_wrapper(wrapperP c)
{
    for(int i=0;i<6;++i) {
        if(!wrappers_[i] || c->from_type_info().get_decoration()==decoration(i)) wrappers_[i] = c->get_wrapper(decoration(i));
        else if(c->from_type_info().get_decoration()==d_const_pointer && i==d_pointer) wrappers_[i] = c->get_wrapper(decoration(i));
    }
}

detail::wrapperP detail::decorated_wrapper::get_wrapper(decoration d)
{
    return wrappers_[d];
}
