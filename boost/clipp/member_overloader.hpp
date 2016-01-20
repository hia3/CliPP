// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_MEMBER_OVERLOADER_HPP_HOLT_24082003
#define BOOST_CLIPP_MEMBER_OVERLOADER_HPP_HOLT_24082003

#include "boost/clipp/member.hpp"
#include <list>

namespace boost { namespace clipp {

class BOOST_CLIPP_EXPORT_IMPORT member_overloader : public member  
{
public:
    member_overloader(member* rhs);
    member_overloader();
    virtual ~member_overloader() {}

    virtual valueP call(Params& arguments,valueP parent=NULL);
    virtual valueP construct(Params& arguments,valueP parent=NULL);
    virtual valueP lookup(const std::string& identifier,valueP parent);
    virtual valueP duplicate(valueP parent=NULL);
    virtual valueP assign(valueP rhs,valueP parent=NULL);
    virtual bool   validate_arguments(value_method method,Params& arguments,valueP parent=NULL);
    virtual valueP join(valueP rhs);
    virtual valueP join_client(valueP rhs);
    virtual type_detail type();
    virtual std::ostream& store(std::ostream& stream);
    virtual valueP wrap_value(valueP parent);
private:
    type_detail type_;
    typedef std::list<valueP> Overloads;
    Overloads overloads_;
};

}} // namespace boost::clipp

#endif //BOOST_CLIPP_MEMBER_OVERLOADER_HPP_HOLT_24082003
