// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_VALUE_HPP_HOLT_29072003
#define BOOST_CLIPP_VALUE_HPP_HOLT_29072003

#include <boost/clipp/config.hpp>
#include <boost/clipp/type_id.hpp>
#include <boost/clipp/ref_base.hpp>
#include <boost/clipp/decoration.hpp>

#include <string>
#include <vector>

namespace boost { namespace clipp {

namespace detail {
    struct converter_base;
    typedef intrusive_ptr<converter_base> converterP;
} // namespace detail

class context;

class value;
typedef clipp_ptr<value> valueP;

struct value_iterator;

class BOOST_CLIPP_EXPORT_IMPORT value : public ref_base
{
public:
    value();
    virtual ~value();
    
    typedef std::vector<valueP> Params;
    enum value_method {
        call_method=1,
        construct_method=2,
        assign_method=4,
        duplicate_method=8
    };

    virtual value_iterator begin() const;
    virtual value_iterator end() const;
    virtual size_t         size() const;

    virtual type_detail type();
    ///Lookup identifier. Returns nullptr if none found
    virtual valueP lookup(const std::string& identifier,valueP parent = nullptr);
    ///Treat value as a function and call it. Return nullptr if not a function
    virtual valueP call(Params& args,valueP parent = nullptr);
    ///Treat value as a constructor and call it. Return nullptr if not a constructor
    virtual valueP construct(Params& args,valueP parent = nullptr);
    ///Duplicate value
    virtual valueP duplicate(valueP parent = nullptr);
    ///Assign a new value.
    virtual valueP assign(valueP rhs,valueP parent = nullptr);
    ///Wrap the value with a parent
    virtual valueP wrap_value(valueP parent);

    virtual valueP insert(const std::string& identifier,valueP value);
    virtual void erase(valueP value);

    virtual valueP join(valueP rhs);
    virtual valueP join_client(valueP rhs);

    virtual valueP prototype();

    ///Validates the input arguments. Used when function overloading in place
    virtual bool   validate_arguments(value_method method,Params& arguments,valueP parent = nullptr);

    ///Convert this value to the type given by type info. Returns nullptr on failure.
    virtual detail::converterP get_converter_from_this(const type_detail& to,precedence p,valueP& wrapped);

    virtual bool is_free() const;

    virtual std::ostream& store(std::ostream& stream);

    virtual void create(context* c) {context_=c;}
    bool created() const {return context_ != nullptr;}
    context* get_context() const {return context_;}
private:
    context* context_;
};

}} // namespace boost::clipp

#endif //BOOST_CLI_VALUE_HPP_HOLT_29072003
