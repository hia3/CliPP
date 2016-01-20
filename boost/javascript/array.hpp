// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_JAVASCRIPT_ARRAY_HPP_HOLT_15092003
#define BOOST_JAVASCRIPT_ARRAY_HPP_HOLT_15092003

#include "boost/javascript/config.hpp"
#include "boost/clipp/container_wrapper.hpp"
#include <boost/javascript/primitive.hpp>
#include <limits>
#include <valarray>
#include <vector>

namespace boost { namespace javascript {

class reference;
class array;
using boost::clipp::valueP;
typedef boost::clipp::clipp_ptr<array> arrayP;
typedef boost::clipp::clipp_ptr<reference> referenceP;

class BOOST_JAVASCRIPT_EXPORT_IMPORT array : public clipp::container_wrapper<std::vector<referenceP> >  
{
public:
    typedef clipp::container_wrapper<container_type> base;
    array(unsigned int length);
    array(std::valarray<clipp::valueP> const &rhs);
    array(const clipp::container_base& container);
    array(const container_type& container);

    clipp::valueP defaultValue(PreferredType::Hint hint=PreferredType::Hint::No);

    std::string toString() const;
    std::string toLocaleString() const;

    virtual clipp::valueP lookup(const std::string& identifier,clipp::valueP parent = nullptr);


    static arrayP concat(std::valarray<clipp::valueP> const &items);
    std::string join(std::string const& separator=",") const;
    clipp::valueP pop();
    int push(std::valarray<clipp::valueP> const &items);
    arrayP reverse();
    clipp::valueP shift();
    arrayP slice(int start,int end=std::numeric_limits<int>::max());
    arrayP sort(clipp::valueP compareFn = nullptr);
    arrayP splice(int start,int deleteCount,std::valarray<clipp::valueP> const &items);
    int unshift(std::valarray<clipp::valueP> const &items);

    unsigned int length() const;
    reference& operator[](unsigned int index);
    valueP operator[](const std::string& identifier);
public:
    array(const array& rhs);
    virtual ~array() {}
    static void init(clipp::context* c);

};

}} // namespace boost::clipp

#endif //BOOST_JAVASCRIPT_NUMBER_HPP_HOLT_15092003

