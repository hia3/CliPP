// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#include <boost/clipp/context.hpp>
#include <boost/clipp/class.hpp>

using namespace boost::clipp;

class_base::class_base(const std::string& name,context* c,const type_detail& type)
:   context_(c)
{
    prototype_ = get_context()->add_prototype(type,name);
}

class_base::class_base(context* c,const type_detail& type)
:   context_(c)
{
    prototype_ = get_context()->add_prototype(type);
}

void class_base::set_base_class(const type_detail& type)
{    
    prototype_->add_base(get_context()->get_prototype(type));
}
