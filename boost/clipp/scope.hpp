// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_OBJECT_HPP_HOLT_28092003
#define BOOST_CLIPP_OBJECT_HPP_HOLT_28092003

#include "boost/clipp/object.hpp"
#include <map>
#include <list>

namespace boost { namespace clipp {

class BOOST_CLIPP_EXPORT_IMPORT scope : public object
{
public:
    scope();
    virtual ~scope();

    virtual value_iterator begin() const;
    virtual value_iterator end() const;
    virtual size_t         size() const;

    virtual valueP lookup(const std::string& identifier,valueP parent = nullptr);
    virtual valueP insert(const std::string& identifier,valueP value);
    virtual void erase(valueP value);
    void clear();

    static void init(context* c);
    virtual std::ostream& store(std::ostream& stream);
private:
    typedef std::map<std::string,valueP> NameMap;
    NameMap values_;
    typedef std::list<std::string> NameStack;
    NameStack stack_;
};

}} // namespace boost::clipp

#endif //BOOST_CLIPP_OBJECT_HPP_HOLT_28092003
