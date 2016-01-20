// Copyright David Abrahams 2002. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_OTHER_HOLT_20092003
# define BOOST_CLIPP_OTHER_HOLT_20092003

# include <boost/clipp/config.hpp>

namespace boost { namespace clipp {

template<class T> struct other
{ 
    typedef T type;
};

# ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
namespace detail
{
  template<typename T>
  class is_other
  {
   public:
      BOOST_STATIC_CONSTANT(bool, value = false); 
  };

  template<typename T>
  class is_other<other<T> >
  {
   public:
      BOOST_STATIC_CONSTANT(bool, value = true);
  };

  template<typename T>
  class unwrap_other
  {
   public:
      typedef T type;
  };

  template<typename T>
  class unwrap_other<other<T> >
  {
   public:
      typedef T type;
  };
}
# else // no partial specialization

}} // namespace boost::clipp

#include <boost/type.hpp>

namespace boost { namespace clipp {

namespace detail
{
  typedef char (&yes_other_t)[1];
  typedef char (&no_other_t)[2];
      
  no_other_t is_other_test(...);

  template<typename T>
  yes_other_t is_other_test(type< other<T> >);

  template<bool wrapped>
  struct other_unwrapper
  {
      template <class T>
      struct apply
      {
          typedef T type;
      };
  };

  template<>
  struct other_unwrapper<true>
  {
      template <class T>
      struct apply
      {
          typedef typename T::type type;
      };
  };

  template<typename T>
  class is_other
  {
   public:
      BOOST_STATIC_CONSTANT(
          bool, value = (
              sizeof(detail::is_other_test(type<T>()))
              == sizeof(detail::yes_other_t)));
  };

  template <typename T>
  class unwrap_other
      : public detail::other_unwrapper<
      is_other<T>::value
  >::template apply<T>
  {};
}

# endif // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

}} // namespace boost::clipp

#endif // BOOST_CLIPP_OTHER_HOLT_20092003
