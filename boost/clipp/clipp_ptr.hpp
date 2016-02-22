// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#pragma once

#include <boost/intrusive_ptr.hpp>
#include <boost/clipp/config.hpp>
#include <vector>

namespace boost {namespace clipp {

class value;
template<typename T>
class clipp_ptr;

typedef clipp_ptr<value> valueP;
typedef std::vector<valueP> Params;


template<typename T>
class clipp_ptr : public intrusive_ptr<T> {
    typedef intrusive_ptr<T> this_type;
public:
    clipp_ptr(){}
    clipp_ptr(T * p, bool add_ref = true) :intrusive_ptr<T>(p,add_ref){}

#if !defined(BOOST_NO_MEMBER_TEMPLATES) || defined(BOOST_MSVC6_MEMBER_TEMPLATES)
    template<class U> clipp_ptr(clipp_ptr<U> const & rhs) :intrusive_ptr<T>(rhs) {}
#endif

    clipp_ptr(clipp_ptr const & rhs): intrusive_ptr<T>(rhs) {}

#if !defined(BOOST_NO_MEMBER_TEMPLATES) || defined(BOOST_MSVC6_MEMBER_TEMPLATES)
    template<class U> clipp_ptr & operator=(clipp_ptr<U> const & rhs)
    {        
        this_type(rhs).swap(*this);
        return *this;
    }
#endif

    clipp_ptr & operator=(clipp_ptr const & rhs)
    {
        this_type(rhs).swap(*this);
        return *this;
    }

    clipp_ptr & operator=(T * rhs)
    {
        this_type(rhs).swap(*this);
        return *this;
    }

    clipp_ptr operator[](const std::string& identifier) 
    {
        return this->get()->lookup(identifier);
    }

    template<typename... A>
    clipp_ptr operator()(A && ... a)
    {
        Params params{wrap(a, this->get()->get_context())...};
        return this->get()->call(params);
    }

};

}} //namespace boost::clipp
