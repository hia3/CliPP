// Copyright David Abrahams 2002. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.
#ifndef BOOST_CLIPP_SELF_HOLT_20092003
# define BOOST_CLIPP_SELF_HOLT_20092003

# include <boost/clipp/config.hpp>
# include <boost/clipp/detail/operators_base.hpp>
#include <boost/call_traits.hpp>

namespace boost { namespace clipp {

#define BOOST_CLIPP_SELF_IS_CLASS

// Sink self_t into its own namespace so that we have a safe place to
// put the completely general operator templates which operate on
// it. It is possible to avoid this, but it turns out to be much more
// complicated and finally GCC 2.95.2 chokes on it.
namespace self_ns
{
# ifndef BOOST_CLIPP_SELF_IS_CLASS
  enum self_t { self };
# else 
  struct self_t {
    inline detail::operator_<'=',self_t,self_t>
    operator=(self_t& t) {
        return detail::operator_<'=',self_t,self_t>();
    }
    template <class R>
    inline detail::operator_<'=',self_t,R>
    operator=(R const& t) {
      return detail::operator_<'=',self_t,R>();
    }
    template <class R>
    inline detail::operator_<'[]',self_t,R>
    operator[](R const& t) {
      return detail::operator_<'[]',self_t,R>();
    }
  };
  extern BOOST_CLIPP_EXPORT_IMPORT self_t self;
# endif
}

using self_ns::self_t;
using self_ns::self;

}} // namespace boost::clipp

#endif // BOOST_CLIPP_SELF_HOLT_20092003

