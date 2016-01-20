// Copyright David Abrahams 2002. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.
#ifndef BOOST_CLIPP_OPERATORS_HOLT_20092003
# define BOOST_CLIPP_OPERATORS_HOLT_20092003

# include <boost/clipp/config.hpp>

# include <boost/clipp/detail/operator_id.hpp>
# include <boost/clipp/detail/not_specified.hpp>
# include <boost/mpl/if.hpp>
# include <boost/clipp/self.hpp>
# include <boost/clipp/other.hpp>
# include <boost/clipp/detail/operators_base.hpp>
# include <boost/lexical_cast.hpp>
# include <boost/clipp/wrap.hpp>
# include <boost/clipp/back_reference.hpp>
# include <string>

namespace boost { namespace clipp { 

# define BOOST_CLIPP_BINARY_OPERATION(id, rid, expr,op_id)  \
namespace detail                                            \
{                                                           \
  template <>                                               \
  struct operator_l<op_id>                                \
  {                                                         \
      template <class L, class R>                           \
      struct apply                                          \
      {                                                     \
          typedef mpl::list<valueP (*)(back_reference<L const&>,R const&),valueP,free_function_tag,cv_unqualified,back_reference<L const&>,R const&> signature_type;\
          static inline valueP execute(back_reference<L const&> l, R const& r) \
          {                                                 \
              return wrap(expr,l.get_context());            \
          }                                                 \
      };                                                    \
      static char const* name() { return "__" #id "__"; }   \
  };                                                        \
                                                            \
  template <>                                               \
  struct operator_r<op_id>                                \
  {                                                         \
      template <class L, class R>                           \
      struct apply                                          \
      {                                                     \
          typedef mpl::list<valueP (*)(back_reference<L const&>,R const&),valueP,free_function_tag,cv_unqualified,back_reference<L const&>,R const&> signature_type;\
          static inline valueP execute(back_reference<L const&> l,R const& r) \
          {                                                 \
              return wrap(expr,l.get_context());            \
          }                                                 \
      };                                                    \
      static char const* name() { return "__" #rid "__"; }  \
  };                                                        \
} 

# define BOOST_CLIPP_BINARY_OPERATOR(id, rid, op,opid)  \
BOOST_CLIPP_BINARY_OPERATION(id, rid, l.get() op r,opid)  \
namespace self_ns                                       \
{                                                       \
  template <class L, class R>                           \
  inline detail::operator_<opid,L,R>    \
  operator op(L const&, R const&)                       \
  {                                                     \
      return detail::operator_<opid,L,R>(); \
  }                                                     \
}
 
BOOST_CLIPP_BINARY_OPERATOR(add, radd, +,'+')
BOOST_CLIPP_BINARY_OPERATOR(sub, rsub, -,'-')
BOOST_CLIPP_BINARY_OPERATOR(mul, rmul, *,'*')
BOOST_CLIPP_BINARY_OPERATOR(div, rdiv, /,'/')
BOOST_CLIPP_BINARY_OPERATOR(mod, rmod, %,'%')
BOOST_CLIPP_BINARY_OPERATOR(lshift, rlshift, <<,'<<')
BOOST_CLIPP_BINARY_OPERATOR(rshift, rrshift, >>,'>>')
BOOST_CLIPP_BINARY_OPERATOR(and, rand, &,'&')
BOOST_CLIPP_BINARY_OPERATOR(xor, rxor, ^,'^')
BOOST_CLIPP_BINARY_OPERATOR(or, ror, |,'|')
BOOST_CLIPP_BINARY_OPERATOR(gt, lt, >,'>')
BOOST_CLIPP_BINARY_OPERATOR(ge, le, >=,'>=')
BOOST_CLIPP_BINARY_OPERATOR(lt, gt, <,'<')
BOOST_CLIPP_BINARY_OPERATOR(le, ge, <=,'<=')
BOOST_CLIPP_BINARY_OPERATOR(eq, req, ==,'==')
BOOST_CLIPP_BINARY_OPERATOR(ne, rne, !=,'!=')
 
# undef BOOST_CLIPP_BINARY_OPERATOR
# undef BOOST_CLIPP_BINARY_OPERATION
    
# define BOOST_CLIPP_INPLACE_OPERATOR(id, op,opid)                   \
namespace detail                                                \
{                                                               \
  template <>                                                   \
  struct operator_l<opid>                       \
  {                                                             \
      template <class L, class R>                               \
      struct apply                                              \
      {                                                         \
          typedef mpl::list<valueP (*)(back_reference<L&>,R const&),valueP,free_function_tag,cv_unqualified,back_reference<L&>,R const&> signature_type;\
          static inline valueP                                  \
          execute(back_reference<L&> l, R const& r)             \
          {                                                     \
              l.get() op r;                                     \
              return l.get_value();                             \
          }                                                     \
      };                                                        \
      static char const* name() { return "__" #id "__"; }       \
  };                                                            \
}                                                               \
namespace self_ns                                               \
{                                                               \
  template <class R>                                            \
  inline detail::operator_<opid,self_t,R>       \
  operator op(self_t const&, R const&)                          \
  {                                                             \
      return detail::operator_<opid,self_t,R>();\
  }                                                             \
}

BOOST_CLIPP_INPLACE_OPERATOR(iadd,+=,'+=')
BOOST_CLIPP_INPLACE_OPERATOR(isub,-=,'-=')
BOOST_CLIPP_INPLACE_OPERATOR(imul,*=,'*=')
BOOST_CLIPP_INPLACE_OPERATOR(idiv,/=,'/=')
BOOST_CLIPP_INPLACE_OPERATOR(imod,%=,'%=')
BOOST_CLIPP_INPLACE_OPERATOR(ilshift,<<=,'<<=')
BOOST_CLIPP_INPLACE_OPERATOR(irshift,>>=,'>>=')
BOOST_CLIPP_INPLACE_OPERATOR(iand,&=,'&=')
BOOST_CLIPP_INPLACE_OPERATOR(ixor,^=,'^=')
BOOST_CLIPP_INPLACE_OPERATOR(ior,|=,'|=')

# undef BOOST_CLIPP_INPLACE_OPERATOR

# define BOOST_CLIPP_UNARY_OPERATOR(id, op, opid,func_name)  \
namespace detail                                                \
{                                                               \
  template <>                                                   \
  struct operator_1<opid>                                    \
  {                                                             \
      template <class T>                                        \
      struct apply                                              \
      {                                                         \
          typedef typename mpl::list<valueP (*)(back_reference<T const &>),valueP,free_function_tag,cv_unqualified,back_reference<T const &> > signature_type;\
          static valueP execute(back_reference<T const &> x)      \
          {                                                     \
              return wrap(op(x.get()),x.get_context());         \
          }                                                     \
      };                                                        \
      static char const* name() { return "__" #id "__"; }       \
  };                                                            \
}                                                               \
namespace self_ns                                               \
{                                                               \
  inline detail::operator_<opid>                     \
  func_name(self_t const&)                                      \
  {                                                             \
      return detail::operator_<opid>();              \
  }                                                             \
}

# define BOOST_CLIPP_UNARY_INPLACE_OPERATOR(id, op, opid, func_name)  \
namespace detail                                                \
{                                                               \
  template <>                                                   \
  struct operator_1<opid>                                    \
  {                                                             \
      template <class T>                                        \
      struct apply                                              \
      {                                                         \
          typedef typename mpl::list<valueP (*)(back_reference<T&>),valueP,free_function_tag,cv_unqualified,back_reference<T&> > signature_type;\
          static valueP execute(back_reference<T &> x)      \
          {                                                     \
              op(x.get());                                     \
              return x.get_value();                             \
          }                                                     \
      };                                                        \
      static char const* name() { return "__" #id "__"; }       \
  };                                                            \
}                                                               \
namespace self_ns                                               \
{                                                               \
  inline detail::operator_<opid>                     \
  func_name(self_t const&)                                      \
  {                                                             \
      return detail::operator_<opid>();              \
  }                                                             \
}

BOOST_CLIPP_UNARY_OPERATOR(neg, -, '-', operator-)
BOOST_CLIPP_UNARY_OPERATOR(pos, +, '+', operator+)
BOOST_CLIPP_UNARY_OPERATOR(invert, ~, '~', operator~)
BOOST_CLIPP_UNARY_OPERATOR(not, !, '!', operator!)
BOOST_CLIPP_UNARY_INPLACE_OPERATOR(inc, ++, '++', operator++)
BOOST_CLIPP_UNARY_INPLACE_OPERATOR(dec, --, '--', operator--)

# undef BOOST_CLIPP_UNARY_OPERATOR

# define BOOST_CLIPP_POSTFIX_OPERATOR(id, op, opid,expr, func_name)  \
namespace detail                                                \
{                                                               \
  template <>                                                   \
  struct operator_l<opid>                       \
  {                                                             \
      template <class L,class R>                                \
      struct apply                                              \
      {                                                         \
          typedef typename mpl::list<valueP (*)(back_reference<L&>),valueP,free_function_tag,cv_unqualified,back_reference<L&> > signature_type;\
          static valueP execute(back_reference<L&> x)     \
          {                                                     \
              return wrap(expr,x.get_context());                \
          }                                                     \
      };                                                        \
      static char const* name() { return "__" #id "__"; }       \
  };                                                            \
}                                                               \
namespace self_ns                                               \
{                                                               \
  inline detail::operator_<opid,self_t,int>     \
  func_name(self_t const&,int)                                  \
  {                                                             \
      return detail::operator_<opid,self_t,int>();\
  }                                                             \
}

BOOST_CLIPP_POSTFIX_OPERATOR(pinc, ++, '++', x.get()++, operator++)
BOOST_CLIPP_POSTFIX_OPERATOR(pdec, --, '--', x.get()--, operator--)

#undef BOOST_CLIPP_POSTFIX_OPERATOR

/*
    op_urshift,   // >>>
    op_iurshift,  // >>>=
    op_seq,       // ===
    op_sne,       // !==
*/

}} // namespace boost::clipp

#endif // BOOST_CLIPP_OPERATORS_HOLT_20092003
