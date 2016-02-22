// Copyright Peder Holt 2003-2004. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#pragma once

#include <boost/clipp/member_base.hpp>
#include <boost/clipp/detail/register_types.hpp>
#include <boost/clipp/detail/signature_extracter.hpp>
#include <boost/clipp/detail/tuple.hpp>

namespace boost { namespace clipp {

template<typename Signature, typename DerivedT>
struct member_with_arguments : public member_base<DerivedT>
{
    typedef Signature signature_type;
    BOOST_STATIC_CONSTANT(int, arity = detail::signature_arity<Signature>::value);
    constexpr int get_arity() const { return arity; }

    explicit member_with_arguments(const char* name)
        : member_base<DerivedT>(name)
    {}

    ///Generic invocation of arguments.
    template<typename ArgumentListT, typename GeneratorT>
    typename GeneratorT::result_type
    invoke(const ArgumentListT& arguments, GeneratorT const& generator = nullptr)
    {
        return invoke_impl(arguments, generator, std::make_index_sequence<arity>{});
    }
    virtual void create(context* c)
    {
        member_base<DerivedT>::create(c);
        invoke(empty_argument_list(), detail::register_types());
    }

    //Define the signature of this function/constructor
    template<typename... Args>
    DerivedT& signature(Args && ... args)
    {
        static_assert(sizeof...(Args) == arity - 1 /*this*/, "");

        for_each
        (
            std::forward_as_tuple(std::forward<Args>(args)...),
            [this](auto n, auto && e)
            {
                std::get<decltype(n)::value>(_a).signature(std::forward<decltype(e)>(e));
            }
        );

        return this->derived();
    }

    //Return a formatted argument list
    std::string format_argument_list()
    {
        std::string format;
        for_each(_a, [&format, c = this->get_context()](auto n, auto & e) { format += (n.value ? ", " : "") + e.format(c); });
        return format;
    }

    //Get the default value at a given position.
    valueP get_default_value(size_t index)
    {
        valueP result;
        for_each(_a, [&result, index, c = this->get_context()](auto n, auto & e) { if (index == n.value) { result = e.get_default_value(c); }; });
        return result;
    }

private:
    template<typename ArgumentListT, typename GeneratorT, std::size_t... index>
    typename GeneratorT::result_type
    invoke_impl(const ArgumentListT& arguments, GeneratorT const& generator, std::index_sequence<index...>)
    {
        return generator.execute
        (
            this->get_context(),
            detail::return_type_wrapper<Signature>(),
            detail::data_type<Signature>(),
            detail::cv_type<Signature>(),
            detail::argument<Signature, index>(arguments)...
        );
    }

    static auto get_doc_args()
    {
        return get_doc_args(std::make_index_sequence<arity - 1>{});
    }
    template<std::size_t... index>
    static auto get_doc_args(std::index_sequence<index...>)
    {
        return std::tuple<doc_arg<Signature, index + 1>...>();
    }
    decltype(get_doc_args()) _a;
};

}} //namespace boost::clipp
