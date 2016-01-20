// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_MEMBER_HPP_HOLT_03082003
#define BOOST_CLIPP_MEMBER_HPP_HOLT_03082003

#include "boost/clipp/value.hpp"

namespace boost { namespace clipp {

class context;

class BOOST_CLIPP_EXPORT_IMPORT member : public value  
{
public:
    member(const char* name);
	virtual ~member();
    valueP join(valueP rhs);
    virtual bool is_free() const;
    const std::string& name() const {return name_;}
    void set_name(const char* name) {name_=name;}
    virtual valueP wrap_value(valueP parent);
private:
    std::string name_;
};

}} // namespace boost::clipp

#endif //BOOST_CLIPP_MEMBER_HPP_HOLT_03082003
