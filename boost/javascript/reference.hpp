// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_REFERENCE_HPP_HOLT_28092003
#define BOOST_CLIPP_REFERENCE_HPP_HOLT_28092003

#include "boost/clipp/object.hpp"
#include "boost/javascript/config.hpp"

namespace boost { namespace javascript {

using boost::clipp::valueP;
using boost::clipp::context;

class BOOST_JAVASCRIPT_EXPORT_IMPORT reference : public clipp::object
{
public:
    reference() {}
    reference(valueP const& rhs);
    reference(reference const& rhs);
    virtual ~reference() {}

    reference& operator=(valueP value);
    reference& operator*=(valueP value);
    reference& operator%=(valueP value);
    reference& operator/=(valueP value);
    reference& operator+=(valueP value);
    reference& operator-=(valueP value);
    reference& operator<<=(valueP value);
    reference& operator>>=(valueP value);
    reference& operator&=(valueP value);
    reference& operator^=(valueP value);
    reference& operator|=(valueP value);
    reference& operator--();
    reference& operator++();
    valueP operator--(int);
    valueP operator++(int);
    virtual valueP duplicate(valueP parent=NULL);
    virtual valueP assign(valueP rhs,valueP parent=NULL);
    virtual valueP call(Params& args,valueP parent=NULL);
    virtual valueP construct(Params& args,valueP parent=NULL);
    virtual clipp::value_iterator begin() const;
    virtual clipp::value_iterator end() const;
    virtual size_t         size() const;

    virtual valueP lookup(const std::string& identifier,valueP parent=NULL);
    virtual valueP wrap_value(valueP parent);
    
    static void init(context* c);

    virtual void create(context* c);

    virtual clipp::detail::converterP get_converter_from_this(const clipp::type_detail& to,clipp::precedence p,valueP& wrapped);
private:
    valueP value_;
};

class BOOST_JAVASCRIPT_EXPORT_IMPORT reference_wrapper : public clipp::object
{
public:
    reference_wrapper(valueP value,valueP parent);
    virtual ~reference_wrapper();
    virtual valueP duplicate(valueP parent=NULL);
    virtual valueP assign(valueP rhs,valueP parent=NULL);
    virtual valueP call(Params& args,valueP parent=NULL);
    virtual valueP construct(Params& args,valueP parent=NULL);
    virtual valueP lookup(const std::string& identifier,valueP parent=NULL);
    virtual clipp::detail::converterP get_converter_from_this(const clipp::type_detail& to,clipp::precedence p,valueP& wrapped);
    virtual clipp::value_iterator begin() const;
    virtual clipp::value_iterator end() const;
    virtual size_t         size() const;
    virtual void create(context* c);
    virtual valueP join_client(valueP rhs);
private:
    valueP value_;
    valueP parent_;
};

}} // namespace boost::clipp

#endif //BOOST_CLIPP_OBJECT_HPP_HOLT_28092003

