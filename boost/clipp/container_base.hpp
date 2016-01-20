// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_CONTAINER_BASE_HPP_HOLT_09112003
#define BOOST_CLIPP_CONTAINER_BASE_HPP_HOLT_09112003

#include <boost/clipp/object.hpp>
#include <boost/clipp/value_iterator.hpp>
#include <iterator>
namespace boost { namespace clipp {

class BOOST_CLIPP_EXPORT_IMPORT container_base : public object  
{
public:
    container_base() {}
    virtual ~container_base() {}

    virtual value_iterator begin() const = 0;
    virtual value_iterator end() const = 0;
    virtual size_t         size() const = 0;

    static void init(context* c);
};

}} // namespace boost::clipp

#endif //BOOST_CLIPP_CONTAINER_BASE_HPP_HOLT_09112003

