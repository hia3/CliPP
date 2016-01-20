#include <boost/clipp/config.hpp>
#include <boost/clipp/context.hpp>
#include <boost/clipp/member_property_wrapper.hpp>
#include <boost/clipp/member_table.hpp>
#include <boost/clipp/detail/operators.hpp>

using namespace boost::clipp;

context::context()
{
    member_property_wrapper::init(this);
    type_names_[typeid(valueP)]="value";
}

context::~context()
{
}

detail::wrapperP context::get_wrapper(const type_detail& from)
{
    detail::wrapperP result=NULL;
    DecoratedWrappers::iterator it=wrappers_.find(from);
    if(it!=wrappers_.end()) result=it->second.get_wrapper(from.get_decoration());
    return result;
}

void context::add_wrapper(detail::wrapperP wrapper)
{
    wrappers_[wrapper->from_type_info()].add_wrapper(wrapper);
}

member_table* context::add_prototype(const type_detail& info,const std::string& name)
{
    member_tableP& table = member_tables_[info];
    if(!table) {
        table=member_tableP(new member_table(info,name));
        table->create(this);
        type_names_[info]=name;
    }
    return table.get();
}

member_table* context::add_prototype(const type_detail& info)
{
    member_tableP& table = member_tables_[info];
    if(!table) {
        table=member_tableP(new member_table(info));
        table->create(this);
    }
    return table.get();
}


member_table* context::get_prototype(const type_detail& info)
{
    MemberTables::iterator it=member_tables_.find(info);
    if(it!=member_tables_.end()) return it->second.get();
    return NULL;
}

void context::set_prototype(const type_detail& info,member_table* prototype)
{
    member_tables_[info]=prototype;
}

void context::add_type_name(const type_detail& info,const std::string& name)
{
    type_names_[info]=name;
}

const char* context::type_name(const type_detail& info)
{
    TypeNames::iterator it=type_names_.find(info);
    if(it!=type_names_.end()) return it->second.c_str();
    else {
        Synonyms::iterator isynonym=synonyms_.find(info);
        if(isynonym!=synonyms_.end()) return type_name(isynonym->second);
        else return info.name();
    }
}

valueP context::lookup(const std::string& identifier)
{
    return global()->lookup(identifier);
}

void context::add_synonym(const type_detail& info,const type_detail& synonym)
{
    synonyms_[info]=synonym;
}
