// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_SET_WRAPPER_HPP_HOLT_07082003
#define BOOST_CLIPP_SET_WRAPPER_HPP_HOLT_07082003

#include "boost/clipp/object.hpp"

namespace boost { namespace clipp {

template<class T>
class set_wrapper : public object  
{
public:
	set_wrapper();
	virtual ~set_wrapper();

private:
    T m_set;
};

}} // namespace boost::clipp

#endif //BOOST_CLIPP_SET_WRAPPER_HPP_HOLT_03082003
