// member.cpp: implementation of the member class.
//
//////////////////////////////////////////////////////////////////////

#include <boost/clipp/member.hpp>
#include <boost/clipp/member_overloader.hpp>
#include <boost/clipp/member_wrapper.hpp>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
using namespace boost::clipp;

member::member(const char* name)
: name_(name)
{
}

member::~member()
{
}

valueP member::join(valueP rhs)
{
    member_overloader* overloader=new member_overloader(this);
    overloader->create(get_context());
    return overloader->join(rhs);
}

bool member::is_free() const
{
    return false;
}

valueP member::wrap_value(valueP parent)
{
    valueP result = new member_wrapper(this,parent);
    result->create(get_context());
    return result;
}
