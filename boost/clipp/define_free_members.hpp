#if !defined (BOOST_PP_IS_ITERATING)

// Copyright Peder Holt 2003-2004. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_DEFINE_FREE_MEMBERS_HPP_HOLT_07032004
#define BOOST_CLIPP_DEFINE_FREE_MEMBERS_HPP_HOLT_07032004

#include <boost/clipp/config.hpp>
#include <boost/clipp/detail/list.hpp>
#include <boost/clipp/detail/signature_extracter.hpp>
#include <boost/clipp/detail/signature.hpp>
#include <boost/clipp/member_function.hpp>

#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing.hpp>

namespace boost { namespace clipp {

#define BOOST_PP_ITERATION_PARAMS_1 (3, (0, BOOST_CLIPP_MAX_ARITY, "boost/clipp/define_free_members.hpp"))
#include BOOST_PP_ITERATE()

template<typename Fn,typename Signature>
member* inner_define_function(Fn fn,Signature const&) {
    return new member_function<Signature>("",fn);
}

template<typename Fn,typename Signature>
member* inner_define_static_function(Fn fn,Signature const&) {
    return new member_static_function<Signature>("",fn);
}

template<typename Fn>
member* function(Fn fn) {
    return inner_define_function(fn,detail::get_signature(fn));
}

template<typename Fn>
member* static_function(Fn fn) {
    return inner_define_static_function(fn,detail::get_signature(fn));
}

namespace detail {
    template<typename Fn,typename Member>
    Member& define_global_member(context* c,const char* name,Fn fn,type<Member> const&) {
        Member* m = new Member(name,fn);
        m->create(c);
        c->global()->insert(name,m);
        return *m;
    }

    template<typename T>
    member& read_selector(context* c,const char* name,T fn,free_data_tag) {
        typedef mpl::list<T,T,free_data_tag,cv_unqualified> signature_type;
        return define_global_member(c,name,fn,boost::type<static_property_read<signature_type> >());
    }
    //Member variable
    template<typename R,typename CT>
    member_property_read<mpl::list<R CT::*,R,member_data_tag,cv_unqualified,CT&> >&
    read_selector(context* c,const char* name,R CT::*var,member_data_tag) {
        return define_global_member(c,name,var,type<member_property_read<mpl::list<R CT::*,R,member_data_tag,cv_unqualified,CT&> > >());
    }

    //Member function
    template<typename R,typename CT>
    member_property_read<mpl::list<R (CT::*)(),R,member_function_tag,cv_unqualified,CT&> >&
    read_selector(context* c,const char* name,R (CT::*fn)(),member_function_tag) {
        return define_global_member(c,name,fn,type<member_property_read<mpl::list<R (CT::*)(),R,member_function_tag,cv_unqualified,CT&> > >());
    }
    template<typename R,typename CT>
    member_property_read<mpl::list<R (CT::*)() const,R,member_function_tag,const_,CT const&> >&
    read_selector(context* c,const char* name,R (CT::*fn)() const,member_function_tag) {
        return define_global_member(c,name,fn,type<member_property_read<mpl::list<R (CT::*)() const,R,member_function_tag,const_,CT const&> > >());
    }

    //Free function
    template<typename R>
    static_property_read<mpl::list<R (*)(),R,free_function_tag,cv_unqualified> >&
    read_selector(context* c,const char* name,R (*fn)(),free_function_tag) {
        return define_global_member(c,name,fn,type<static_property_read<mpl::list<R (*)(),R,free_function_tag,cv_unqualified> > >());
    }
    //Free function as member
    template<typename R,typename CT>
    member_property_read<mpl::list<R (*)(CT),R,free_function_tag,cv_unqualified,CT> >&
    read_selector(context* c,const char* name,R (*fn)(CT),free_function_tag) {
        return define_global_member(c,name,fn,type<member_property_read<mpl::list<R (*)(CT),R,free_function_tag,cv_unqualified,CT> > >());
    }
    template<typename T>
    static_property_read<mpl::list<T&,T&,reference_wrapper_tag,cv_unqualified> >&
    read_selector(context* c,const char* name,const reference_wrapper<T>& var,reference_wrapper_tag) {
        return define_global_member(c,name,var,type<static_property_read<mpl::list<T&,T&,reference_wrapper_tag,cv_unqualified> > >());
    }
}

template<typename Fn>
member& read(context* c,const char* name,Fn fn) {
    return detail::read_selector(c,name,fn,detail::get_data_tag(fn));
};

//Member variable
template<typename R,typename CT>
member_property_write<mpl::list<R CT::*,R,detail::member_data_tag,detail::cv_unqualified,CT&,R> >&
write(context* c,const char* name,R CT::*var) {
    return detail::define_global_member(c,name,var,type<member_property_write<mpl::list<R CT::*,R,detail::member_data_tag,detail::cv_unqualified,CT&,R> > >());
}

//Member function
template<typename R,typename CT,typename A0>
member_property_write<mpl::list<R (CT::*)(A0),R,detail::member_function_tag,detail::cv_unqualified,CT&,A0> >&
write(context* c,const char* name,R (CT::*fn)(A0)) {
    return detail::define_global_member(c,name,fn,type<member_property_write<mpl::list<R (CT::*)(A0),R,detail::member_function_tag,detail::cv_unqualified,CT&,A0> > >());
}
template<typename R,typename CT,typename A0>
member_property_write<mpl::list<R (CT::*)(A0) const,R,detail::member_function_tag,detail::const_,CT const&,R,A0> >&
write(context* c,const char* name,R (CT::*fn)(A0) const) {
    return detail::define_global_member(c,name,fn,type<member_property_write<mpl::list<R (CT::*)(A0) const,R,detail::member_function_tag,detail::const_,CT const&,A0> > >());
}

//Free function
template<typename R,typename A0>
static_property_write<mpl::list<R (*)(A0),R,detail::free_function_tag,detail::cv_unqualified,A0> >&
write(context* c,const char* name,R (*fn)(A0)) {
    return detail::define_global_member(c,name,fn,type<static_property_write<mpl::list<R (*)(A0),R,detail::free_function_tag,detail::cv_unqualified,A0> > >());
}
//Free function as member
template<typename R,typename CT,typename A0>
member_property_write<mpl::list<R (*)(CT,A0),R,detail::free_function_tag,detail::cv_unqualified,CT,A0> >&
write(context* c,const char* name,R (*fn)(CT,A0)) {
    return detail::define_global_member(c,name,fn,type<member_property_write<mpl::list<R (*)(CT,A0),R,detail::free_function_tag,detail::cv_unqualified,CT,A0> > >());
}

template<typename T>
static_property_write<mpl::list<T&,T&,detail::reference_wrapper_tag,detail::cv_unqualified,const T&> >&
write(context* c,const char* name,const reference_wrapper<T>& var) {
    return detail::define_global_member(c,name,var,type<static_property_write<mpl::list<T&,T&,detail::reference_wrapper_tag,detail::cv_unqualified,const T&> > >());
}

template<typename Read,typename Write>
member& read_write(context* c,const char* name,Read r,Write w) {
    read(c,name,r);
    return write(c,name,w);
}

}} //namespace boost::clipp

#endif //BOOST_CLIPP_DEFINE_FREE_MEMBERS_HPP_HOLT_07032004

#else //BOOST_PP_IS_ITERATING

#define N BOOST_PP_ITERATION()

    //static Free function
    template<typename R BOOST_PP_ENUM_TRAILING_PARAMS(N, typename A)>
    member_static_function<mpl::list<R (*)(BOOST_PP_ENUM_PARAMS(N, A)),R,detail::free_function_tag,detail::cv_unqualified BOOST_PP_ENUM_TRAILING_PARAMS(N, A)> >&
    function(context* c,const char* name,R (*fn)(BOOST_PP_ENUM_PARAMS(N, A))) 
    {
        typedef member_static_function<mpl::list<R (*)(BOOST_PP_ENUM_PARAMS(N, A)),R,detail::free_function_tag,detail::cv_unqualified BOOST_PP_ENUM_TRAILING_PARAMS(N, A)> > member_type;

        member_type* m = new member_type(name,fn);
        m->create(c);
        c->global()->insert(name,m);
        return *m;
    }

/*    template<typename R BOOST_PP_ENUM_TRAILING_PARAMS(N, typename A)>
    member* function(R (*fn)(BOOST_PP_ENUM_PARAMS(N, A))) 
    {
        typedef member_static_function<mpl::list<R (*)(BOOST_PP_ENUM_PARAMS(N, A)),R,detail::free_function_tag,detail::cv_unqualified BOOST_PP_ENUM_TRAILING_PARAMS(N, A)> > member_type;
        return new member_type(name,fn);
    }

    template<typename CT,typename R BOOST_PP_ENUM_TRAILING_PARAMS(N, typename A)>
    member* function(R (CT::*fn)(BOOST_PP_ENUM_PARAMS(N, A))) 
    {
        typedef member_function<mpl::list<R (CT::*)(BOOST_PP_ENUM_PARAMS(N, A)),R,detail::member_function_tag,detail::cv_unqualified,CT& BOOST_PP_ENUM_TRAILING_PARAMS(N, A)> > member_type;
        return new member_type("",fn);
    }

    template<typename CT,typename R BOOST_PP_ENUM_TRAILING_PARAMS(N, typename A)>
    member* function(R (CT::*fn)(BOOST_PP_ENUM_PARAMS(N, A)) const) 
    {
        typedef member_function<mpl::list<R (CT::*)(BOOST_PP_ENUM_PARAMS(N, A)) const ,R,detail::member_function_tag,detail::const_,CT& BOOST_PP_ENUM_TRAILING_PARAMS(N, A)> > member_type;
        return new member_type("",fn);
    }

    template<typename R BOOST_PP_ENUM_TRAILING_PARAMS(N, typename A)>
    member* static_function(R (*fn)(BOOST_PP_ENUM_PARAMS(N, A))) 
    {
        typedef member_static_function<mpl::list<R (*)(BOOST_PP_ENUM_PARAMS(N, A)),R,detail::free_function_tag,detail::cv_unqualified BOOST_PP_ENUM_TRAILING_PARAMS(N, A)> > member_type;
        return new member_type("",fn);
    }

    template<typename CT,typename R BOOST_PP_ENUM_TRAILING_PARAMS(N, typename A)>
    member* static_function(R (CT::*fn)(BOOST_PP_ENUM_PARAMS(N, A))) 
    {
        typedef member_static_function<mpl::list<R (CT::*)(BOOST_PP_ENUM_PARAMS(N, A)),R,detail::member_function_tag,detail::cv_unqualified,CT& BOOST_PP_ENUM_TRAILING_PARAMS(N, A)> > member_type;
        return new member_type("",fn);
    }

    template<typename CT,typename R BOOST_PP_ENUM_TRAILING_PARAMS(N, typename A)>
    member* static_function(R (CT::*fn)(BOOST_PP_ENUM_PARAMS(N, A)) const) 
    {
        typedef member_static_function<mpl::list<R (CT::*)(BOOST_PP_ENUM_PARAMS(N, A)) const,R,detail::member_function_tag,detail::const_,CT& BOOST_PP_ENUM_TRAILING_PARAMS(N, A)> > member_type;
        return new member_type("",fn);
    }
*/
#undef N

#endif //BOOST_PP_IS_ITERATING

