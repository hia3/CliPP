// Copyright Peder Holt 2003-2004. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_CLASS_BUILDER_HPP_HOLT_27052004
#define BOOST_CLIPP_CLASS_BUILDER_HPP_HOLT_27052004

#include <boost/clipp/class_wrapper.hpp>
#include <boost/clipp/define_constructor.hpp>
#include <boost/clipp/define_function.hpp>
#include <boost/clipp/define_read.hpp>
#include <boost/clipp/define_write.hpp>
#include <boost/clipp/define_operator.hpp>
#include <boost/clipp/define_converter.hpp>

#include <type_traits>

namespace boost { namespace clipp {

namespace detail {

template<typename C,typename T1,typename T2,typename T3>
struct select_storage_policy {
    typedef typename mpl::if_<
        std::is_convertible<C*,value*>,
        default_storage_policy,
        typename mpl::if_<
            std::is_convertible<T1,storage_policy_tag>,
            T1,
            typename mpl::if_<
                std::is_convertible<T2,storage_policy_tag>,
                T2,
                typename mpl::if_<
                    std::is_convertible<T3,storage_policy_tag>,
                    T3,
                    direct_storage_policy
                >::type
            >::type
        >::type
    >::type type;
};

template<typename C,typename T1>
struct select_base_type {
    typedef typename mpl::if_<
        is_base_and_derived<T1,C>,
        T1,
        mpl::void_
    >::type type;
};

template<typename C,typename T1,typename T2>
struct select_wrapped_type {
    typedef typename mpl::if_<
        std::is_convertible<C*,value*>,
        typename mpl::if_<
            is_same<typename select_base_type<C,T1>::type,mpl::void_>,
            T1,
            T2
        >::type,
        C
    >::type func_;

    typedef typename mpl::if_<
        mpl::or_<
            is_same<func_,mpl::void_>,
            is_base_and_derived<policy_tag,func_>
        >,
        mpl::void_,
        func_
    >::type type;
};

}

class BOOST_CLIPP_EXPORT_IMPORT class_base
{
public:
    class_base(const std::string& name,context* c,const type_detail& type);
    class_base(context* c,const type_detail& type);
    context* get_context() const {return context_;}
    void set_base_class(const type_detail& type);
    member_table* prototype() const {return prototype_;}
private:
    member_table* prototype_;
    context* context_;
};

template<typename C,typename StorageP,typename StorageT,typename BaseT,typename WrappedT,typename DerivedT>
struct class_implementation
:   public class_base
,   public detail::define_constructor<C,DerivedT,StorageP>
,   public detail::define_function<C,DerivedT>
,   public detail::define_read<C,DerivedT>
,   public detail::define_write<C,DerivedT>
,   public detail::define_operator<C,DerivedT>
,   public detail::define_converter<C,DerivedT>
{
public:
    typedef StorageP storage_policy;
    typedef StorageT storage_type;
    typedef BaseT base_type;
    typedef WrappedT wrapped_type;

    class_implementation(const std::string& name,context* c) 
    : class_base(name,c,type_id<StorageT>())
    {
        //Add constructors to global namespace
        if(get_context()->global()) get_context()->global()->insert(name,prototype());
    }
    class_implementation(context* c) 
    : class_base(c,type_id<StorageT>())
    {
    }
};

template<class C,typename T1,typename T2,typename T3,typename DerivedT>
struct class_builder {
    typedef typename detail::select_storage_policy<C,T1,T2,T3>::type storage_policy;
    
    typedef typename policy_apply<storage_policy,C>::type storage_type;
    typedef typename detail::select_base_type<C,T1>::type base_type;
    typedef typename detail::select_wrapped_type<C,T1,T2>::type wrapped_type;
    typedef class_implementation<C,storage_policy,storage_type,base_type,wrapped_type,DerivedT> type;
};

}} //namespace boost::clipp

#endif
