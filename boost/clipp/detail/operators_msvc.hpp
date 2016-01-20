// Copyright Peder Holt 2003-2004. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_OPERATORS_MSVC_HPP_HOLT_01032004
#define BOOST_CLIPP_OPERATORS_MSVC_HPP_HOLT_01032004

namespace boost { namespace clipp { namespace detail {

template<operator_id id>
struct msvc_op_l {
    template<typename L,typename R>
    struct apply{};
};

template<operator_id id>
struct msvc_op_r {
    template<typename L,typename R>
    struct apply{};
};

template<operator_id id>
struct msvc_op_1 {
    template<typename T>
    struct apply{};
};

#define BOOST_CLIPP_MSVC_OP(op_id)                          \
template<>                                                  \
struct msvc_op_l<op_id>                                     \
{                                                           \
    template<typename L,typename R>                         \
    struct apply{                                           \
        typedef mpl::list<valueP (*)(back_reference<L const&>,R const&),valueP,free_function_tag,cv_unqualified,back_reference<L const&>,R const&> signature_type;\
    };                                                      \
};                                                          \
template<>                                                  \
struct msvc_op_r<op_id>                                     \
{                                                           \
    template<typename L,typename R>                         \
    struct apply{                                           \
        typedef mpl::list<valueP (*)(back_reference<L const&>,R const&),valueP,free_function_tag,cv_unqualified,back_reference<L const&>,R const&> signature_type;\
    };                                                      \
};                                                          

BOOST_CLIPP_MSVC_OP('+');
BOOST_CLIPP_MSVC_OP('-');
BOOST_CLIPP_MSVC_OP('*');
BOOST_CLIPP_MSVC_OP('/');
BOOST_CLIPP_MSVC_OP('%');
BOOST_CLIPP_MSVC_OP('<<');
BOOST_CLIPP_MSVC_OP('>>');
BOOST_CLIPP_MSVC_OP('&');
BOOST_CLIPP_MSVC_OP('^');
BOOST_CLIPP_MSVC_OP('|');
BOOST_CLIPP_MSVC_OP('>');
BOOST_CLIPP_MSVC_OP('>=');
BOOST_CLIPP_MSVC_OP('<');
BOOST_CLIPP_MSVC_OP('<=');
BOOST_CLIPP_MSVC_OP('==');
BOOST_CLIPP_MSVC_OP('!=');
#undef BOOST_CLIPP_MSVC_OP

#define BOOST_CLIPP_MSVC_OP(op_id)                          \
template<>                                                  \
struct msvc_op_l<op_id>                                     \
{                                                           \
    template<typename L,typename R>                         \
    struct apply{                                           \
        typedef mpl::list<valueP (*)(back_reference<L&>,R const&),valueP,free_function_tag,cv_unqualified,back_reference<L&>,R const&> signature_type;\
    };                                                      \
};                                                          \

BOOST_CLIPP_MSVC_OP('[]');
BOOST_CLIPP_MSVC_OP('=');
BOOST_CLIPP_MSVC_OP('+=');
BOOST_CLIPP_MSVC_OP('-=');
BOOST_CLIPP_MSVC_OP('*=');
BOOST_CLIPP_MSVC_OP('/=');
BOOST_CLIPP_MSVC_OP('%=');
BOOST_CLIPP_MSVC_OP('<<=');
BOOST_CLIPP_MSVC_OP('>>=');
BOOST_CLIPP_MSVC_OP('&=');
BOOST_CLIPP_MSVC_OP('^=');
BOOST_CLIPP_MSVC_OP('|=');
#undef BOOST_CLIPP_MSVC_OP

#define BOOST_CLIPP_MSVC_OP(op_id)                          \
template<>                                                  \
struct msvc_op_1<op_id>                                     \
{                                                           \
    template<typename T>                                    \
    struct apply{                                           \
        typedef typename mpl::list<valueP (*)(back_reference<T const &>),valueP,free_function_tag,cv_unqualified,back_reference<T const &> > signature_type;\
    };                                                      \
};       

BOOST_CLIPP_MSVC_OP('-')
BOOST_CLIPP_MSVC_OP('+')
BOOST_CLIPP_MSVC_OP('~')
BOOST_CLIPP_MSVC_OP('!')
#undef BOOST_CLIPP_MSVC_OP

#define BOOST_CLIPP_MSVC_OP(op_id)                          \
template<>                                                  \
struct msvc_op_1<op_id>                                     \
{                                                           \
    template<typename T>                                    \
    struct apply{                                           \
        typedef typename mpl::list<valueP (*)(back_reference<T&>),valueP,free_function_tag,cv_unqualified,back_reference<T&> > signature_type;\
    };                                                      \
};      

BOOST_CLIPP_MSVC_OP('++')
BOOST_CLIPP_MSVC_OP('--')
#undef BOOST_CLIPP_MSVC_OP

#define BOOST_CLIPP_MSVC_OP(op_id)                          \
template<>                                                  \
struct msvc_op_l<op_id>                                     \
{                                                           \
    template<typename L,typename R>                         \
    struct apply{                                           \
        typedef typename mpl::list<valueP (*)(back_reference<L&>),valueP,free_function_tag,cv_unqualified,back_reference<L&> > signature_type;\
    };                                                      \
};       

BOOST_CLIPP_MSVC_OP('++')
BOOST_CLIPP_MSVC_OP('--')
#undef BOOST_CLIPP_MSVC_OP

template<operator_id id,typename C>
struct msvc_binary_op
{
    typedef typename msvc_op_l<id>::apply<C,C>::signature_type signature_type;
};

template<operator_id id,typename L,typename R>
struct msvc_binary_op_l
{
    typedef typename msvc_op_l<id>::apply<L,R>::signature_type signature_type;
};

template<operator_id id,typename L,typename R>
struct msvc_binary_op_r
{
    typedef typename msvc_op_r<id>::apply<L,R>::signature_type signature_type;
};

template<operator_id id,typename T>
struct msvc_unary_op
{
    typedef typename msvc_op_1<id>::apply<T>::signature_type signature_type;
};

template<operator_id id,typename L,typename R,typename C>
struct msvc_operator_selector {
    typedef typename
        mpl::if_<
            is_same<L,self_t>
            , typename mpl::if_<
                 is_same<R,self_t>
                , msvc_binary_op<id,C>
                , msvc_binary_op_l<id,C,typename detail::unwrap_other<R>::type>
              >::type
            , typename mpl::if_<
                is_same<L,detail::not_specified>
                , msvc_unary_op<id,C>
                , msvc_binary_op_r<id,typename detail::unwrap_other<L>::type,C>
                 >::type
          >::type operator_type;
    typedef typename operator_type::signature_type signature_type;
};

}}} //namespace boost::clipp::detail

#endif //BOOST_CLIPP_OPERATORS_MSVC_HPP_HOLT_01032004
