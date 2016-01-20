#include <boost/clipp/error_messages.hpp>
#include <boost/clipp/context.hpp>

using namespace boost::clipp;

bad_type_conversion::bad_type_conversion(context* c,const type_detail& from,const type_detail& to)
: from_(c->type_name(from))
, to_(c->type_name(to)) 
{}

invalid_argument::invalid_argument(context* c,const std::type_info& from,const std::type_info& to,unsigned int iarg)
: from_(c->type_name(from))
, to_(c->type_name(to)) 
, iarg_(iarg)
{}
