//-----------------------------------------------------------------------------
// boost aligned_storage.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2002-2003
// Eric Friedman, Itay Maman
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_ALIGNED_STORAGE_HPP
#define BOOST_ALIGNED_STORAGE_HPP

#include <cstddef> // for std::size_t

#include "boost/config.hpp"
#include "boost/detail/workaround.hpp"
#include "boost/type_traits/alignment_of.hpp"
#include "boost/type_traits/type_with_alignment.hpp"

#include "boost/mpl/eval_if.hpp"
#include "boost/mpl/identity.hpp"

namespace boost {

namespace detail { namespace aligned_storage {

BOOST_STATIC_CONSTANT(
      std::size_t
    , alignment_of_max_align = ::boost::alignment_of<max_align>::value
    );

}} // namespace detail::aligned_storage

template <
      std::size_t size_
    , std::size_t alignment_ = std::size_t(-1)
>
class aligned_storage
{
private: // representation

    union data_t
    {
        char buf[size_];

        typename mpl::eval_if_c<
              alignment_ == std::size_t(-1)
            , mpl::identity<detail::max_align>
            , type_with_alignment<alignment_>
            >::type align_;
    } data_;

public: // constants

    BOOST_STATIC_CONSTANT(
          std::size_t
        , size = size_
        );
    BOOST_STATIC_CONSTANT(
          std::size_t
        , alignment = (
              alignment_ == std::size_t(-1)
            ? ::boost::detail::aligned_storage::alignment_of_max_align
            : alignment_
            )
        );

#if defined(__GNUC__) &&\
    (__GNUC__ >  3) ||\
    (__GNUC__ == 3 && (__GNUC_MINOR__ >  2 ||\
                      (__GNUC_MINOR__ == 2 && __GNUC_PATCHLEVEL__ >=3)))

private: // noncopyable

    aligned_storage(const aligned_storage&);
    aligned_storage& operator=(const aligned_storage&);

#else // gcc less than 3.2.3

public: // _should_ be noncopyable, but GCC compiler emits error

    aligned_storage(const aligned_storage&);
    aligned_storage& operator=(const aligned_storage&);

#endif // gcc < 3.2.3 workaround

public: // structors

    aligned_storage()
    {
    }

    ~aligned_storage()
    {
    }

public: // accessors

    void* address()
    {
        return &data_.buf[0];
    }

#if !BOOST_WORKAROUND(BOOST_MSVC, <= 1200)

    const void* address() const
    {
        return &data_.buf[0];
    }

#else // MSVC6

    const void* address() const;

#endif // MSVC6 workaround

};

#if BOOST_WORKAROUND(BOOST_MSVC, <= 1200)

// MSVC6 seems not to like inline functions with const void* returns, so we
// declare the following here:

template <std::size_t S, std::size_t A>
const void* aligned_storage<S,A>::address() const
{
    return const_cast< aligned_storage<S,A>* >(this)->address();
}

#endif // MSVC6 workaround

} // namespace boost

#endif // BOOST_ALIGNED_STORAGE_HPP
