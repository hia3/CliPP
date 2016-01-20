// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_DETAIL_OPERATORS_BASE_HPP_HOLT_29092003
#define BOOST_CLIPP_DETAIL_OPERATORS_BASE_HPP_HOLT_29092003

#include <boost/clipp/other.hpp>
//#include <boost/clipp/detail/operator_id.hpp>
#include <boost/clipp/detail/not_specified.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/if.hpp>
#include <boost/clipp/back_reference.hpp>
#include <boost/clipp/detail/signature.hpp>

namespace boost { namespace clipp { 

namespace self_ns
{
  struct self_t;
} // namespace self_ns

using self_ns::self_t;

typedef int operator_id;

namespace detail
{
  // Operator implementation template declarations. The nested apply
  // declaration here keeps MSVC6 happy.
  template <operator_id> struct operator_l
  {
      template <class L, class R> struct apply;
  };
  
  template <operator_id> struct operator_r
  {
      template <class L, class R> struct apply;
  };

  template <operator_id> struct operator_1
  {
      template <class T> struct apply;
  };

  // MSVC6 doesn't want us to do this sort of inheritance on a nested
  // class template, so we use this layer of indirection to avoid
  // ::template<...> on the nested apply functions below
  template <operator_id id, class L, class R>
  struct operator_l_inner
      : operator_l<id>::template apply<L,R>
  {};
      
  template <operator_id id, class L, class R>
  struct operator_r_inner
      : operator_r<id>::template apply<L,R>
  {};

  template <operator_id id, class T>
  struct operator_1_inner
      : operator_1<id>::template apply<T>
  {};
      
  // Define three different binary_op templates which take care of
  // these cases:
  //    self op self
  //    self op R
  //    L op self
  // 
  // The inner apply metafunction is used to adjust the operator to
  // the class type being defined. Inheritance of the outer class is
  // simply used to provide convenient access to the operation's
  // name().

  // self op self
  template <operator_id id>
  struct binary_op : operator_l<id>
  {
      template <class T>
      struct apply : operator_l_inner<id,T,T>
      {
      };
  };

  // self op R
  template <operator_id id, class R>
  struct binary_op_l : operator_l<id>
  {
      template <class T>
      struct apply : operator_l_inner<id,T,R>
      {
      };
  };

  // L op self
  template <operator_id id, class L>
  struct binary_op_r : operator_r<id>
  {
      template <class T>
      struct apply : operator_r_inner<id,L,T>
      {
      };
  };

  template <operator_id id>
  struct unary_op : operator_1<id>
  {
      template <class T>
      struct apply : operator_1_inner<id,T>
      {
      };
  };

  // This type is what actually gets returned from operators used on
  // self_t
  template <operator_id id, class L = not_specified, class R = not_specified>
  struct operator_
      : mpl::if_<
            is_same<L,self_t>
            , typename mpl::if_<
                 is_same<R,self_t>
                , binary_op<id>
                , binary_op_l<id,typename unwrap_other<R>::type>
              >::type
            , typename mpl::if_<
                is_same<L,not_specified>
                , unary_op<id>
                , binary_op_r<id,typename unwrap_other<L>::type>
                 >::type
          >::type
  {
  };

    template <>
    struct operator_l<'='>
    {
          template <class L, class R>
          struct apply
          {
              typedef mpl::list<valueP (*)(back_reference<L&>,R const&),valueP,free_function_tag,cv_unqualified,back_reference<L&>,R const&> signature_type;
              static inline valueP execute(back_reference<L&> l, R const& r)
              {
                  l.get() = r;
                  return l.get_value();
              }
          };
          static char const* name() { return "__assign__"; }
    };          
    template <>
    struct operator_l<'[]'>
    {
          template <class L, class R>
          struct apply
          {
              typedef mpl::list<valueP (*)(back_reference<L&>,R const&),valueP,free_function_tag,cv_unqualified,back_reference<L&>,R const&> signature_type;
              static inline valueP execute(back_reference<L&> l, R const& r)
              {
                  return wrap_ref(l.get()[r],l.get()[r],l.get_context());
              }
          };
          static char const* name() { return "__subscript__"; }
    };          
} //namespace detail

}} //namespace boost::clipp

#endif

