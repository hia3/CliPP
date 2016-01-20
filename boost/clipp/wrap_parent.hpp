// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_WRAP_PARENT_HPP_HOLT_18102003
#define BOOST_CLIPP_WRAP_PARENT_HPP_HOLT_18102003

#include "boost/clipp/value.hpp"

namespace boost { namespace clipp {

class BOOST_CLIPP_EXPORT_IMPORT wrap_parent : public value
{
public:
    wrap_parent(valueP& value,valueP& parent);
    virtual ~wrap_parent() {}

    valueP lookup(const std::string& identifier,valueP parent=NULL) {return value_->lookup(identifier,parent_);}
    virtual valueP call(Params& arguments,valueP parent=NULL)       {return value_->call(arguments,parent_);}
    virtual valueP construct(Params& arguments,valueP parent=NULL)  {return value_->construct(arguments,parent_);}
    virtual valueP duplicate(valueP parent=NULL)                    {return value_->duplicate(parent_);}
    virtual valueP assign(valueP rhs,valueP parent=NULL)            {return value_->assign(rhs,parent_);}
    virtual bool   validate_arguments(value_method method,Params& arguments,valueP parent=NULL) 
                                                                    {return value_->validate_arguments(method,arguments,parent_);}
    virtual valueP prototype() {return value_;}
    virtual detail::converterP get_converter_from_this(const type_detail& to,precedence p,valueP& wrapped);
private:
    valueP value_;
    valueP parent_;
};

}} // namespace boost::clipp

#endif //BOOST_CLIPP_WRAP_PARENT_HPP_HOLT_18102003

