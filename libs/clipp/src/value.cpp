// value.cpp: implementation of the value class.
//
//////////////////////////////////////////////////////////////////////

#include <boost/clipp/value.hpp>
#include <boost/clipp/error_messages.hpp>
#include <boost/clipp/detail/converter.hpp>
#include <boost/clipp/value_iterator.hpp>
#include <stdexcept>

#include <set>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
using namespace boost::clipp;

value::value()
:   context_(nullptr)
{
}

value::~value()
{

}

valueP value::lookup(const std::string& identifier,valueP parent)
{
    if(prototype()) return prototype()->lookup(identifier,parent);
    return nullptr;
}

valueP value::call(Params& args,valueP parent)
{
    return nullptr;
}

valueP value::construct(Params& args,valueP parent)
{
    return nullptr;
}

valueP value::duplicate(valueP parent)
{
    return this;
}

valueP value::assign(valueP rhs,valueP parent)
{
    return nullptr;
}

valueP value::prototype()
{
    return nullptr;
}

type_detail value::type()
{
    return typeid(*this);
}

bool value::validate_arguments(value_method /*method*/,Params& arguments,valueP parent)
{
    return false;
}

valueP value::join(valueP rhs)
{
    return nullptr;
}

valueP value::join_client(valueP rhs)
{
    return rhs->join(this);
}

detail::converterP value::get_converter_from_this(const type_detail& to,precedence p,valueP& wrapped)
{
    return nullptr;
}

valueP value::insert(const std::string& identifier,valueP value)
{
    throw std::runtime_error("This members interface is read-only");
}

void value::erase(valueP value)
{
    throw std::runtime_error("This members interface is read-only");
}

bool value::is_free() const
{
    return true;
}

value_iterator value::begin() const
{
    throw std::runtime_error("Value has no members");
}

value_iterator value::end() const
{
    throw std::runtime_error("Value has no members");
}

size_t value::size() const
{
    return 0;
}

std::ostream& value::store(std::ostream& stream)
{
    return stream;
}

valueP value::wrap_value(valueP parent)
{
    return this;
}
