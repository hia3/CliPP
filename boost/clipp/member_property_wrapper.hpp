// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_MEMBER_PROPERTY_WRAPPER_HPP_HOLT_25102003
#define BOOST_CLIPP_MEMBER_PROPERTY_WRAPPER_HPP_HOLT_25102003

#include <boost/clipp/member_wrapper.hpp>

namespace boost { namespace clipp {

class BOOST_CLIPP_EXPORT_IMPORT member_property_wrapper : public member_wrapper  
{
public:
    member_property_wrapper(const valueP& property,const valueP& parent) : member_wrapper(property,parent) {}
    virtual ~member_property_wrapper() {}
   
    static void init(context* c);

    valueP operator=(valueP rhs);
    valueP operator+=(valueP rhs);
    valueP operator-=(valueP rhs);
    valueP operator*=(valueP rhs);
    valueP operator/=(valueP rhs);
    valueP operator<<=(valueP rhs);
    valueP operator>>=(valueP rhs);
    valueP operator&=(valueP rhs);
    valueP operator^=(valueP rhs);
    valueP operator|=(valueP rhs);
};

}} // namespace boost::clipp

#endif //BOOST_CLIPP_MEMBER_PROPERTY_WRAPPER_HPP_HOLT_25102003

