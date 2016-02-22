// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#include <boost/clipp/member_table.hpp>
#include <boost/clipp/member_property_wrapper.hpp>
#include <boost/clipp/error_messages.hpp>
#include <boost/clipp/context.hpp>
#include <stdexcept>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
using namespace boost::clipp;
namespace boost { namespace clipp {
class member_property_base;
}}//namespace boost::clipp

member_table::member_table(const type_detail& info,const std::string& name)
:   base_(nullptr)
,   info_(info)
,   name_(name)
{

}

member_table::member_table(const type_detail& info)
:   base_(nullptr)
,   info_(info)
{

}

member_table::~member_table()
{
}

valueP member_table::lookup(const std::string& identifier,valueP parent)
{
    MemberTable::iterator it=member_table_.find(identifier);
    if(it!=member_table_.end()) return it->second;
    else return value::lookup(identifier,parent);
}

bool member_table::validate_arguments(value_method method,Params& arguments,valueP parent)
{
    MemberTable::iterator it = member_table_.find("constructor");
    if(it!=member_table_.end()) return it->second->validate_arguments(method,arguments,parent);
    else return false;
}

valueP member_table::call(Params& args,valueP parent)
{
    MemberTable::iterator it = member_table_.find("constructor");
    if(it!=member_table_.end()) return it->second->call(args,parent);
    else throw std::runtime_error("No constructors found");
}

valueP member_table::construct(Params& args,valueP parent)
{
    MemberTable::iterator it = member_table_.find("constructor");
    if(it!=member_table_.end()) return it->second->construct(args,parent);
    else throw std::runtime_error("No constructors found");
}

valueP member_table::insert(const std::string& identifier,valueP member)
{
    valueP& old = member_table_[identifier];
    if(old) old = old->join(member);
    else    old = member;
    return  old;
}

valueP member_table::prototype()
{
    return base_;
}

bool member_table::add_base(member_table* base)
{
    base_=base;
    return true;
}

void member_table::add_converter_to_this(detail::converterP c)
{
    converters_to_this_[c->from_type_info()]=c;
}

void member_table::add_converter_from_this(detail::converterP c)
{
    converters_from_this_[c->to_type_info()].add_converter(c);
}

void member_table::set_wrap_table(member_table* table)
{
    wrap_table_=table;
}

detail::converterP member_table::get_converter_to_this(const type_detail& from)
{
    Converters::iterator it=converters_to_this_.find(from);
    if(it==converters_to_this_.end()) {
        if(wrap_table_) {
            if(wrap_table_->converters_to_this_.find(from)!=wrap_table_->converters_to_this_.end()) {
                for(DecoratedConverters::iterator id=wrap_table_->converters_from_this_.begin();id!=wrap_table_->converters_from_this_.end();++id) {
                    it=converters_to_this_.find(id->first);
                    if(it!=converters_to_this_.end()) return it->second;
                }
            }
        }
        return nullptr;
    }
    return it->second;
}

detail::converterP member_table::get_converter_from_this(const type_detail& to,precedence p,valueP& wrapped)
{
    detail::converterP result=find_converter_from_this(to,p);
    if(!result && p== precedence::create) return create_converter_from_this(to,p);
    return result;
}

detail::converterP member_table::find_converter_from_this(const type_detail& to,precedence p)
{
    detail::converterP result = nullptr;
    DecoratedConverters::iterator it=converters_from_this_.find(to);
    if(it!=converters_from_this_.end()) result=it->second.get_converter(to.get_decoration());
    if(!result && bases()) result=bases()->find_converter_from_this(to,p);
    return result;
}

detail::converterP member_table::create_converter_from_this(const type_detail& to,precedence p)
{
    detail::converterP result = nullptr;
    member_table* to_table=get_context()->get_prototype(to);
    if(to_table) {
        detail::converterP c=to_table->get_converter_to_this(info_);
        if(c) {
            add_converter_from_this(c);
            result=c->get_converter(to.get_decoration());
        }
    }
    if(!result && bases()) return bases()->create_converter_from_this(to,p);
    return result;
}

std::ostream& member_table::store(std::ostream& stream)
{
    stream << "class " << name();
    if(base_) stream << " : public " << base_->name();
    stream << "{" << std::endl;
    for(MemberTable::iterator it=member_table_.begin();it!=member_table_.end();++it) {
        valueP member=it->second;
        member->store(stream);
    }
    stream << "};" << std::endl;
    return stream;
}
