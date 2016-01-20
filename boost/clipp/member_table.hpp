// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_MEMBER_TABLE_HPP_HOLT_03082003
#define BOOST_CLIPP_MEMBER_TABLE_HPP_HOLT_03082003

#include "boost/clipp/value.hpp"
#include "boost/clipp/detail/decorated_converter.hpp"
#include <map>

namespace boost { namespace clipp {

class BOOST_CLIPP_EXPORT_IMPORT member_table : public value  
{
public:
    member_table(const type_detail& info,const std::string& name);
    member_table(const type_detail& info);
	virtual ~member_table();

    ///Lookup identifier. Returns NULL if none found
    virtual valueP lookup(const std::string& identifier,valueP parent=NULL);
    virtual valueP prototype();
    virtual valueP insert(const std::string& identifier,valueP member);
    virtual bool add_base(member_table* base);
    virtual void set_wrap_table(member_table* table);

    ///Validates the input arguments. Used when function overloading in place
    virtual bool   validate_arguments(value_method method,Params& arguments,valueP parent=NULL);
    ///Treat value as a function and call it. Return NULL if not a function
    virtual valueP call(Params& args,valueP parent=NULL);
    ///Treat value as a constructor and call it. Return NULL if not a constructor
    virtual valueP construct(Params& args,valueP parent=NULL);

    void add_converter_to_this(detail::converterP converter);
    void add_converter_from_this(detail::converterP converter);
    detail::converterP get_converter_to_this(const type_detail& from);
    detail::converterP get_converter_from_this(const type_detail& to,precedence p,valueP& wrapped);

    detail::converterP find_converter_from_this(const type_detail& to,precedence p);
    detail::converterP create_converter_from_this(const type_detail& to,precedence p);

    virtual std::ostream& store(std::ostream& stream);
    
    const std::string& name() {return name_;}
    virtual type_detail info() {return info_;}

    member_table* bases() {return static_cast<member_table*>(base_);}
private:
    typedef std::map<std::string,valueP> MemberTable;
    MemberTable member_table_;
    typedef std::map<type_detail,detail::converterP> Converters;
    typedef std::map<type_detail,detail::decorated_converter> DecoratedConverters;

    DecoratedConverters  converters_from_this_;
    Converters           converters_to_this_;

    clipp_ptr<member_table> wrap_table_;
    member_table* base_;

    type_detail info_;
    std::string             name_;
};

}} // namespace boost::clipp

#endif //BOOST_CLIPP_MEMBER_TABLE_HPP_HOLT_03082003

