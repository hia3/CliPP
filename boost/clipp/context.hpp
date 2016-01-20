// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_CONTEXT_HPP_HOLT_03082003
#define BOOST_CLIPP_CONTEXT_HPP_HOLT_03082003

#include <boost/clipp/config.hpp>
#include <boost/clipp/type_id.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/clipp/detail/decorated_wrapper.hpp>
#include <map>
#include <set>
#include <string>
#include <boost/clipp/member_table.hpp>

namespace boost { namespace clipp {

class value;
class member_table;

class BOOST_CLIPP_EXPORT_IMPORT context 
{
public:
    context();
    virtual ~context();

    detail::wrapperP get_wrapper(const type_detail& from);
    void add_wrapper(detail::wrapperP wrapper);

    member_table* add_prototype(const type_detail& info,const std::string& name);
    member_table* add_prototype(const type_detail& info);
    member_table* get_prototype(const type_detail& info);
    void set_prototype(const type_detail& info,member_table* prototype);

    void add_synonym(const type_detail& info,const type_detail& synonym);

    void add_type_name(const type_detail& info,const std::string& name);
    const char* type_name(const type_detail& info);
    void setGlobal(valueP global) {global_=global;}
    void setNull(valueP null) {null_=null;}
    valueP global() {return global_;}
    valueP null() {return null_;}

    valueP lookup(const std::string& identifier);

private:
    typedef std::map<type_detail,detail::decorated_wrapper> DecoratedWrappers;
    DecoratedWrappers  wrappers_;

    typedef clipp_ptr<member_table> member_tableP;
    typedef std::map<type_detail,member_tableP> MemberTables;
    MemberTables   member_tables_;
    typedef std::map<type_detail,type_detail> Synonyms;

    typedef std::map<type_detail,std::string> TypeNames;
    TypeNames      type_names_;
    Synonyms       synonyms_;
    valueP         global_;
    valueP         null_;
};

template<typename T>
bool is_type_defined(context* c,T* dummy = nullptr) {
    return c->get_prototype(type_id<T>()) != nullptr;
}

}} // namespace boost::clipp

#endif //BOOST_CLIPP_CONTEXT_HPP_HOLT_03082003

