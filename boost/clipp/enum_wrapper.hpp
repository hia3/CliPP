// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_ENUM_WRAPPER_HPP_HOLT_31082003
# define BOOST_CLIPP_ENUM_WRAPPER_HPP_HOLT_31082003

#include <boost/clipp/object.hpp>

namespace boost { namespace clipp { 

template<typename EnumT>
class enum_wrapper : public object {
public:
    enum_wrapper(EnumT const& rhs) : value_(rhs) {}
    operator EnumT const&() {return value_;}
private:
    EnumT value_;
};

}} //namespace boost::clipp::detail

#endif //BOOST_CLIPP_ENUM_WRAPPER_HPP_HOLT_31082003

