// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_OBJECT_HPP_HOLT_03082003
#define BOOST_CLIPP_OBJECT_HPP_HOLT_03082003

#include <boost/clipp/value.hpp>

namespace boost { namespace clipp {

class BOOST_CLIPP_EXPORT_IMPORT object : public value  
{
public:
    object();
    virtual ~object();

    virtual void create(context* c);

    virtual detail::converterP get_converter_from_this(const type_detail& to,precedence p,valueP& wrapped);

    virtual valueP operator[](const std::string& identifier);
    virtual valueP lookup(const std::string& identifier,valueP parent = nullptr);
    virtual valueP construct(Params& args,valueP parent = nullptr);

    virtual valueP prototype();

    static void init(context* c);
private:
    value* member_table_;
};

}} // namespace boost::clipp

#endif //BOOST_CLIPP_OBJECT_HPP_HOLT_03082003
