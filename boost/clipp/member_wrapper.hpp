// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_MEMBER_WRAPPER_HPP_HOLT_20102003
#define BOOST_CLIPP_MEMBER_WRAPPER_HPP_HOLT_20102003

#include "boost/clipp/object.hpp"

namespace boost { namespace clipp {

class BOOST_CLIPP_EXPORT_IMPORT member_wrapper : public object  
{
public:
    member_wrapper(const valueP& value,const valueP& parent);
    virtual ~member_wrapper() {}
    virtual valueP lookup(const std::string& identifier,valueP parent = nullptr);
    virtual valueP call(Params& arguments,valueP parent = nullptr);
    virtual valueP construct(Params& arguments,valueP parent = nullptr);
    virtual valueP duplicate(valueP parent = nullptr);
    virtual valueP assign(valueP rhs,valueP parent = nullptr);
    virtual bool   validate_arguments(value_method method,Params& arguments,valueP parent = nullptr);
    virtual detail::converterP get_converter_from_this(const type_detail& to,precedence p,valueP& wrapped);
    static void init(context* c);
    virtual valueP join_client(valueP rhs);
private:
    valueP value_;
    valueP parent_;
};

}} // namespace boost::clipp

#endif //BOOST_CLIPP_MEMBER_WRAPPER_HPP_HOLT_20102003

