// Copyright Peder Holt 2003-2004. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_DEFINE_CONVERTER_HPP_HOLT_27052004
#define BOOST_CLIPP_DEFINE_CONVERTER_HPP_HOLT_27052004

#include <boost/clipp/config.hpp>
#include <boost/clipp/detail/list.hpp>
#include <boost/clipp/detail/signature_extracter.hpp>
#include <boost/clipp/detail/cv_category.hpp>

namespace boost { namespace clipp {

template<typename From,typename To>
struct convertible_from_to {
    typedef typename mpl::if_<
        is_convertible<From const&,To>,
        From const&,
        typename mpl::if_<
            is_convertible<From&,To>,
            From&,
            typename mpl::if_<
                is_convertible<From const*,To>,
                From const*,
                typename mpl::if_<
                    is_convertible<From*,To>,
                    From*,
                    typename mpl::if_<
                        is_convertible<From,To>,
                        From,
                        invalid_conversion<From,To>
                    >::type
                >::type
            >::type
        >::type
    >::type type;
};

template<typename To,typename From>
struct convertible_to_from {
    typedef typename mpl::if_<
        is_convertible<From,To&>,
        To&,
        typename mpl::if_<
            is_convertible<From,To>,
            To,
            typename mpl::if_<
                is_convertible<From,To*>,
                To*,
                typename mpl::if_<
                    is_convertible<From,To const*>,
                    To const*,
                    typename mpl::if_<
                        is_convertible<From,To const&>,
                        To const&,
                        invalid_conversion<From,To>
                    >::type
                >::type
            >::type
        >::type
    >::type type;
};

namespace detail {
/*
template<typename C,typename StorageP,typename DerivedT>
class define_converter {
    typedef StorageP                                        storage_policy;
    typedef typename policy_apply<storage_policy,C>::type   storage_t;
    typedef typename C                                      class_t;
    typedef typename 
        mpl::if_<
            mpl::is_same<class_t,storage_t>,
            class_type,
            convertible_from_to<class_t,storage_t>
        >::type                                             embed_t;

    template<typename U>
    void wrap(type<U> const& u) {
        typedef typename mpl::and<
            is_same<unwrap_type<U>::cv,cv_unqualified>,
            is_same<unwrap_type<U>::indirection,direct_>
        >::type auto_deduce_conversion_type;

        wrap_selector(u,auto_deduce_conversion_type());
    }

    template<typename U>
    void converter(U u) {
        typedef typename DerivedT::storage_type storage_type;
        converter_signature(u,detail::get_signature(u,boost::type<storage_type>()));
    }
private:
    //Auto deduce conversion type
    void wrap_selector(type<U> const& u,mpl::true_) {
        typedef typename convertible_from_to<U,class_type>::type from_type;
        typedef typename convertible_to_from<U,class_type>::type to_type;

        auto_construct(BOOST_CLIPP_UNWRAP_TYPE(from_type),mpl::not_<is_invalid_conversion<from_type> >());
        implicit_convert(BOOST_CLIPP_UNWRAP_TYPE(to_type),mpl::not_<is_invalid_conversion<to_type> >());
        derived().get_context()->add_type_name(type_id<U>(),derived().get_context()->type_name(typeid(storage_type)));
    }
    //No autodeduction of conversion type
    void wrap_selector(type<U> const& u,mpl::true_) {
        typedef typename mpl::if_<
            is_convertible<U,class_type>,
            U,
            invalid_conversion<U,class_type>
        >::type from_type;
        typename mpl::if_<
            is_convertible<class_type,U>,
            U,
            invalid_conversion<class_type,U>
        >::type to_type;

        auto_construct(BOOST_CLIPP_UNWRAP_TYPE(from_type),mpl::not_<is_invalid_conversion<from_type> >());
        implicit_convert(BOOST_CLIPP_UNWRAP_TYPE(to_type),mpl::not_<is_invalid_conversion<to_type> >());
        derived().get_context()->add_type_name(type_id<U>(),derived().get_context()->type_name(typeid(storage_type)));
    }

    //Helper functions
    template<typename FromTraits>
    void auto_construct(FromTraits from,mpl::bool_<true>) {
        typedef mpl::list<embed_type,detail::direct_,detail::cv_unqualified> to_traits;
        derived().get_context()->add_wrapper(new detail::constructor_wrapper<FromTraits,to_traits>());
    }

};
*/
template<typename C,typename DerivedT>
class define_converter {
public:
    define_converter() {
        register_class_converters(BOOST_DEDUCED_TYPENAME boost::type<BOOST_DEDUCED_TYPENAME DerivedT::wrapped_type>());
        register_bases(is_same<BOOST_DEDUCED_TYPENAME DerivedT::base_type,mpl::void_>());
    }

    template<typename U>
    void wrap(type<U> const& t) {
        wrap_impl(t,is_plain<U>());
    }

    template<typename U>
    void converter(U u) {
        typedef typename DerivedT::storage_type storage_type;
        internal_define_converter(u,detail::get_signature(u,boost::type<storage_type>()));
    }

private:
    template<typename U>
    void wrap_impl(type<U> const& t,mpl::true_) {
        typedef typename DerivedT::storage_type storage_type;
        typedef typename convertible_from_to<U,storage_type>::type from_type;
        typedef typename convertible_to_from<U,storage_type>::type to_type;

        wrap_helper(BOOST_CLIPP_UNWRAP_TYPE(from_type),mpl::not_<is_invalid_conversion<from_type> >());
        convert_helper(BOOST_CLIPP_UNWRAP_TYPE(to_type),mpl::not_<is_invalid_conversion<to_type> >());
        derived().get_context()->add_type_name(type_id<U>(),derived().get_context()->type_name(typeid(storage_type)));
    }

    template<typename U>
    void wrap_impl(type<U> const& t,mpl::false_) {
        typedef typename DerivedT::storage_type storage_type;
        typedef typename U from_type;
        typedef typename U to_type;

        wrap_helper(BOOST_CLIPP_UNWRAP_TYPE(from_type),mpl::not_<is_invalid_conversion<from_type> >());
        convert_helper(BOOST_CLIPP_UNWRAP_TYPE(to_type),mpl::not_<is_invalid_conversion<to_type> >());
        derived().get_context()->add_type_name(type_id<U>(),derived().get_context()->type_name(typeid(storage_type)));
    }

    //Register default coversion to and from this class.
    template<typename U>
    void register_class_converters(boost::type<U>const &t) {
        typedef typename DerivedT::storage_type storage_type;
        derived().prototype()->add_converter_from_this(new detail::cast_converter<storage_type>());
        derived().prototype()->add_converter_from_this(new detail::cast_converter<value>());
        derived().get_context()->add_wrapper(new detail::cast_wrapper<storage_type>());
        wrap(t);
    };
    void register_class_converters(boost::type<mpl::void_>) {
        typedef typename DerivedT::storage_type storage_type;
        derived().prototype()->add_converter_from_this(new detail::cast_converter<storage_type>());
        derived().prototype()->add_converter_from_this(new detail::cast_converter<value>());
        derived().get_context()->add_wrapper(new detail::cast_wrapper<storage_type>());
    };
    //Register base classes for this class.
    void register_bases(mpl::true_) {}
    void register_bases(mpl::false_) 
    {
        typedef typename DerivedT::base_type base_type;
        if(detail::register_type(boost::type<base_type>(),derived().get_context())) {
            derived().set_base_class(typeid(base_type));
        }
    }

    //Helper functions
    template<typename FromTraits>
    void wrap_helper(FromTraits from,mpl::bool_<true>) {
        typedef typename DerivedT::storage_type storage_type;
        typedef mpl::list<storage_type,detail::direct_,detail::cv_unqualified> to_traits;
        derived().get_context()->add_wrapper(new detail::constructor_wrapper<FromTraits,to_traits>());
    }
    template<typename ToTraits>
    void convert_helper(ToTraits to,mpl::bool_<true>) {
        typedef typename DerivedT::storage_type storage_type;
        typedef mpl::list<storage_type,detail::direct_,detail::cv_unqualified> from_traits;
        derived().prototype()->add_converter_from_this(new detail::implicit_conversion_converter<from_traits,ToTraits>());
        member_table* wrapped_prototype = derived().get_context()->add_prototype(type_id_traits(to));
        wrapped_prototype->add_converter_to_this(new detail::unwrap_constructor_converter<from_traits,ToTraits>());
        wrapped_prototype->set_wrap_table(derived().prototype());
    }
    template<typename ToTraits>
    void convert_helper(ToTraits to,mpl::bool_<false>) {
        typedef typename DerivedT::wrapped_type wrapped_type;
        typedef typename DerivedT::storage_type storage_type;
        typedef typename detail::unwrap_value_type<ToTraits>::type::to_type input_type;
        BOOST_STATIC_ASSERT((is_same<input_type,wrapped_type>::value==false));
        typedef typename convertible_to_from<wrapped_type,storage_type>::type selected_wrapped_type;
        typedef typename convertible_to_from<input_type,wrapped_type>::type to_type;
        convert_helper_wrapped(BOOST_CLIPP_UNWRAP_TYPE(selected_wrapped_type),BOOST_CLIPP_UNWRAP_TYPE(to_type),mpl::not_<is_invalid_conversion<to_type> >());
    }
    template<typename WrappedTraits,typename ToTraits>
    void convert_helper_wrapped(WrappedTraits wrapped,ToTraits to,mpl::bool_<true>) {
        typedef typename DerivedT::storage_type storage_type;
        typedef mpl::list<storage_type,detail::direct_,detail::cv_unqualified> from_traits;
        derived().prototype()->add_converter_from_this(new detail::implicit_conversion_wrapped_converter<from_traits,WrappedTraits,ToTraits>());
    }
    template<typename WrappedTraits,typename ToTraits>
    void convert_helper_wrapped(WrappedTraits wrapped,ToTraits to,mpl::bool_<false>) {
        unable_to_perform_conversion;
    }

    template<typename U,typename Signature>
    void internal_define_converter(U u,Signature s) {
        detail::data_type<Signature> * dummy = nullptr;
        internal_define_converter(u,dummy,s);
    }
    template<typename U,typename Signature> 
    void internal_define_converter(U u,detail::member_function_tag*,Signature s) {
        BOOST_STATIC_ASSERT(detail::signature_arity<Signature>::value==1);
        typedef typename DerivedT::storage_type from_type;
        typedef detail::return_type<Signature> to_type;
        define_function_call_converter(u,BOOST_CLIPP_UNWRAP_TYPE(from_type),BOOST_CLIPP_UNWRAP_TYPE(to_type));
    }
    template<typename U,typename Signature> 
    void internal_define_converter(U u,detail::constructor_tag*,Signature s) {
        BOOST_STATIC_ASSERT(detail::signature_arity<Signature>::value==1);
        typedef typename detail::argument<Signature,0>::result_type from_type;
        typedef detail::return_type<Signature> to_type;
        define_constructor_converter(BOOST_CLIPP_UNWRAP_TYPE(from_type),BOOST_CLIPP_UNWRAP_TYPE(to_type));
    }
    template<typename Fn,typename FromTraits,typename ToTraits>
    void define_function_call_converter(Fn fn,FromTraits from,ToTraits to)
    {
        derived().prototype()->add_converter_from_this(new detail::function_call_converter<FromTraits,ToTraits,Fn>(fn));
    }
    template<typename FromTraits,typename ToTraits>
    void define_constructor_converter(FromTraits from,ToTraits to)
    {
        typedef typename DerivedT::storage_type storage_type;
        typedef typename DerivedT::wrapped_type wrapped_type;
        if(is_same<storage_type,BOOST_DEDUCED_TYPENAME detail::unwrap_value_type<FromTraits>::type>::value) {
            derived().prototype()->add_converter_from_this(new detail::constructor_converter<FromTraits,ToTraits>());
            derived().get_context()->add_wrapper(new detail::constructor_wrapper<FromTraits,ToTraits>());
        }
        else if(is_same<wrapped_type,BOOST_DEDUCED_TYPENAME detail::unwrap_value_type<FromTraits>::type>::value) {
            derived().get_context()->add_wrapper(new detail::constructor_wrapper<FromTraits,ToTraits>());
        }
        derived().prototype()->add_converter_to_this(new detail::constructor_converter<FromTraits,ToTraits>());
    }
private:
    DerivedT& derived() {return static_cast<DerivedT&>(*this);}
};

} //namespace detail

}} //namespace boost::clipp


#endif //BOOST_CLIPP_DEFINE_CONVERTER_HPP_HOLT_27052004

