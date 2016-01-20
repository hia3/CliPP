// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_CLASS_HPP_HOLT_07082003
#define BOOST_CLIPP_CLASS_HPP_HOLT_07082003

#include <boost/clipp/context.hpp>

#include <boost/clipp/member_table.hpp>
#include <boost/clipp/context.hpp>
#include <boost/clipp/detail/wrapper.hpp>
#include <boost/clipp/class_wrapper.hpp>
#include <boost/clipp/type_id.hpp>
#include <boost/mpl/void.hpp>
#include <boost/static_assert.hpp>
#include <boost/clipp/invalid_conversion.hpp>
#include <boost/clipp/class_builder.hpp>

namespace boost { namespace clipp {

template<class Class>
class function_adder
{
public:
    function_adder(Class* cls,const std::string& name) : cls_(cls) , name_(name) {}
    function_adder(const function_adder& adder) : cls_(adder.cls_), name_(adder.name_) {}

    function_adder& operator=(const function_adder& rhs) {
        name_=rhs.name_;
        cls_=rhs.cls_;
        return *this;
    }

    template<typename T>
    valueP operator=(T t) {
        return valueP(&cls_->function(name_.c_str(),t));
    }
    valueP operator=(member* m) {
        m->create(cls_->get_context());
        m->set_name(name_.c_str());
        cls_->prototype()->insert(name_,m);
        return m;
    }
private:
    std::string name_;
    Class* cls_;
};

template<typename C,typename T1=mpl::void_,typename T2=mpl::void_,typename T3=mpl::void_>
class class_
:   public class_builder<C,T1,T2,T3,class_<C,T1,T2,T3> >::type
{
public:
    typedef typename class_builder<C,T1,T2,T3,class_>::type base;
    //Construct a new class, or retrieve old class definition
    class_(const std::string& name,context* c) : base(name,c){}
    class_(context* c) : base(c) {}
    
    template<typename... A>
    member_constructor<mpl::list<arguments<A...>,C,typename base::storage_policy::create_tag,detail::cv_unqualified, A...> >&
    operator[](const boost::clipp::constructor<A...>& t)
    {
        return base::constructor(t);
    }

    template <operator_id id, class L, class R>

    member_operator<typename detail::operator_<id,L,R>::template apply<C>::signature_type>&
    operator[](detail::operator_<id,L,R> const& op)
    {
        return def_operator(op);
    }
    function_adder<class_> operator[](const std::string& name) {
        return function_adder<class_>(this,name);
    }

    template <operator_id id, class L, class R>

    member_operator<typename detail::operator_<id,L,R>::template apply<C>::signature_type>&
    operator_(detail::operator_<id,L,R> const& op)
    {
        return def_operator(op);
    }

    template<typename T,typename MemberType>
    MemberType& define_member(const char* name,T value,boost::type<MemberType> const& dummy)
    {
        MemberType* member=new MemberType(name,value);
        member->create(get_context());
        prototype()->insert(name,member);
        return *member;
    }
    template<typename MemberType>
    MemberType& define_member(const char* name,MemberType* member)
    {
        member->create(get_context());
        prototype()->insert(name,member);
        return *member;
    }
};

}} // namespace boost::clipp

#include <boost/clipp/container_wrapper.ipp>

#endif //BOOST_CLIPP_CLASS_HPP_HOLT_07082003

