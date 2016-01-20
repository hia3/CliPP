// Copyright Peder Holt 2003-2004. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_MEMBER_BASE_HPP_HOLT_29022004
#define BOOST_CLIPP_MEMBER_BASE_HPP_HOLT_29022004

#include <boost/clipp/argument_lists.hpp>
#include <boost/clipp/argument.hpp>
#include <boost/clipp/member.hpp>

namespace boost { namespace clipp {

template<typename DerivedT>
class member_base : public member
{
public:
    member_base(const char* name) : member(name) {}
    DerivedT& derived() {return static_cast<DerivedT&>(*this);}
};

}} //namespace boost::clipp;

#endif //BOOST_CLIPP_MEMBER_BASE_HPP_HOLT_29022004

