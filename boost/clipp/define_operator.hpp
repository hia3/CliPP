// Copyright Peder Holt 2003-2004. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_DEFINE_OPERATOR_HPP_HOLT_29022004
#define BOOST_CLIPP_DEFINE_OPERATOR_HPP_HOLT_29022004

#include <boost/clipp/config.hpp>
#include <boost/clipp/detail/list.hpp>
#include <boost/clipp/detail/signature_extracter.hpp>
#include <boost/clipp/detail/cv_category.hpp>
#include <boost/clipp/detail/operators.hpp>

#if BOOST_WORKAROUND(BOOST_MSVC, <= 1300)
#include <boost/clipp/detail/operators_msvc.hpp>
#endif

#include <boost/clipp/member_function.hpp>

namespace boost { namespace clipp {

template<typename Signature>
class member_operator : public member_static_function<Signature> 
{
public:
    typedef member_static_function<Signature>  base_type;
    typedef typename base_type::value_type value_type;
    member_operator(const char* name,value_type function) 
    :   base_type(name,function) {}
};

namespace detail {

template<typename C,typename DerivedT>
class define_operator {
public:

#if BOOST_WORKAROUND(BOOST_MSVC, <= 1300)
    template <operator_id id,class L, class R>
    member_operator<typename msvc_operator_selector<id,L,R,C>::signature_type>&
    def_operator(operator_<id,L,R> const& op)
    {
        typedef detail::operator_<id,L,R> op_t;
        typedef typename member_operator<BOOST_DEDUCED_TYPENAME msvc_operator_selector<id,L,R,C>::signature_type> member_type;
        return derived().define_member(op.name(),op_t::template apply<C>::execute,type<member_type>());
    }
#else
    template <operator_id id,class L, class R>
    member_operator<typename operator_<id,L,R>::template apply<C>::signature_type>&
    def_operator(operator_<id,L,R> const& op)
    {
        typedef detail::operator_<id,L,R> op_t;
        typedef member_operator<typename op_t::template apply<C>::signature_type> member_type;
        return derived().define_member(op.name(),&op_t::template apply<C>::execute,type<member_type>());
    }
#endif

private:
    DerivedT& derived() {return static_cast<DerivedT&>(*this);}
};

} //namespace detail

}} //namespace boost::clipp


#endif //BOOST_CLIPP_DEFINE_OPERATOR_HPP_HOLT_29022004

