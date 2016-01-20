// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_VALUE_WRAPPER_HPP_HOLT_12092003
#define BOOST_CLIPP_VALUE_WRAPPER_HPP_HOLT_12092003

#include "boost/clipp/value.hpp"

namespace boost { namespace clipp {

template<typename T>
class value_wrapper : public value  
{
public:
    value_wrapper(const T& t) : t_(t) {}
    virtual ~value_wrapper() {}
    operator const T&() {return t_;}
private:
    T t_;
};

}} // namespace boost::clipp

#endif //BOOST_CLIPP_VALUE_WRAPPER_HPP_HOLT_12092003

