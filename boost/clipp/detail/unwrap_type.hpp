// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_DETAIL_UNWRAP_TYPE_HPP_HOLT_31082003
# define BOOST_CLIPP_DETAIL_UNWRAP_TYPE_HPP_HOLT_31082003

#include <boost/mpl/at.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/clipp/detail/list.hpp>
#include <boost/clipp/detail/cv_category.hpp>
#include <boost/clipp/detail/indirection_category.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/type_traits/add_pointer.hpp>
#include <boost/type_traits/detail/yes_no_type.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/type_traits/remove_reference.hpp>


namespace boost { namespace clipp { namespace detail {

template<typename T>
struct is_const_pointer : mpl::false_ {};

template<typename T>
struct is_const_pointer<T const*> : mpl::true_ {};

template<typename T>
struct is_const_reference : mpl::false_ {};

template<typename T>
struct is_const_reference<T const&> : mpl::true_ {};

template<typename Traits>
struct unwrap_indirection : mpl::at<Traits,mpl::int_<1> > {};

template<typename Traits>
struct unwrap_cv : mpl::at<Traits,mpl::int_<2> > {};

template<typename Traits>
struct unwrap_value_type : mpl::at<Traits,mpl::int_<0> > {};

template<typename Traits>
struct unwrap_decoration {
    typedef typename unwrap_cv<Traits>::type trait_cv;
    typedef typename unwrap_indirection<Traits>::type trait_indirection;
    BOOST_STATIC_CONSTANT(int,value = 
                                4*trait_indirection::is_pointer + 
                                2*trait_indirection::is_reference +
                                trait_cv::is_const
                         );
};

template<typename T>
struct unwrap_base {
    typedef T (*fn)();
};

template<typename T>
struct unwrap_pointer : unwrap_base<T> 
{
    typedef typename unwrap_base<T>::fn fn;
    template<typename U>
        static mpl::list<U,pointer_,cv_unqualified> unwrap(U* (*)())
    {
        return mpl::list<U,pointer_,cv_unqualified>();
    }

    template<typename U>
    static U type_size(U* (*)());
    BOOST_STATIC_CONSTANT(int,size=(sizeof(type_size((fn)0))));

    typedef cv_unqualified cv;
    typedef pointer_       indirection;
};

template<typename T>
struct unwrap_const_pointer : unwrap_base<T> 
{
    typedef typename unwrap_base<T>::fn fn;
    template<typename U>
    static mpl::list<U,pointer_,const_> unwrap(U const* (*)())
    {
        return mpl::list<U,pointer_,const_>();
    }
    template<typename U>
    static U type_size(U const* (*)());
    BOOST_STATIC_CONSTANT(int,size=(sizeof(type_size((fn)0))));

    typedef const_         cv;
    typedef pointer_       indirection;
};

template<typename T>
struct unwrap_reference : unwrap_base<T> 
{
    typedef typename unwrap_base<T>::fn fn;
    template<typename U>
    static mpl::list<U,reference_,cv_unqualified> unwrap(U& (*)())
    {
        return mpl::list<U,reference_,cv_unqualified>();
    }
    template<typename U>
    static U type_size(U& (*)());
    BOOST_STATIC_CONSTANT(int,size=(sizeof(type_size((fn)0))));

    typedef cv_unqualified cv;
    typedef reference_     indirection;
};

template<typename T>
struct unwrap_const_reference : unwrap_base<T> 
{
    typedef typename unwrap_base<T>::fn fn;
    template<typename U>
    static mpl::list<U,reference_,const_> unwrap(U const& (*)())
    {
        return mpl::list<U,reference_,const_>();
    }
    template<typename U>
    static U type_size(U const& (*)());
    BOOST_STATIC_CONSTANT(int,size=(sizeof(type_size((fn)0))));

    typedef const_         cv;
    typedef reference_     indirection;
};

template<typename T>
struct unwrap_direct : unwrap_base<T*> 
{
    typedef typename unwrap_base<T*>::fn fn;
    template<typename U>
    static mpl::list<U,direct_,cv_unqualified> unwrap(U* (*)())
    {
        return mpl::list<U,direct_,cv_unqualified>();
    }
    template<typename U>
    static U type_size(U* (*)());
    BOOST_STATIC_CONSTANT(int,size=(sizeof(type_size((fn)0))));

    typedef cv_unqualified cv;
    typedef direct_        indirection;
};

template<typename T>
struct unwrap_const_direct : unwrap_base<T*> 
{
    typedef typename unwrap_base<T*>::fn fn;
    template<typename U>
    static mpl::list<U,direct_,const_> unwrap(U const* (*)())
    {
        return mpl::list<U,direct_,const_>();
    }
    template<typename U>
    static U type_size(U const* (*)());

    BOOST_STATIC_CONSTANT(int,size=(sizeof(type_size((fn)0))));
    typedef const_         cv;
    typedef direct_        indirection;
};

template<typename T>
struct unwrap_type_selector
{
    typedef typename
        mpl::if_<
            is_pointer<T>,
            typename mpl::if_<
                is_const_pointer<T>,
                unwrap_const_pointer<T>,
                unwrap_pointer<T>
                >::type,
            typename mpl::if_<
                is_reference<T>,
                typename mpl::if_<
                    is_const_reference<T>,
                    unwrap_const_reference<T>,
                    unwrap_reference<T>
                    >::type,
                typename mpl::if_<
                    is_const<T>,
                    unwrap_const_direct<T>,
                    unwrap_direct<T>
                >::type
            >::type
        >::type type;
};

template<typename T>
struct is_plain : mpl::not_<
mpl::or_<is_pointer<T>,is_reference<T>,is_const<T> > > {};

template<typename T>
struct unwrap_type
{
    typedef T type;
    typedef cv_unqualified cv;
    typedef direct_        indirection;
    typedef mpl::list<type,indirection,cv> traits;
};

template<typename T>
struct unwrap_type<T const>
{
    typedef T type;
    typedef const_ cv;
    typedef direct_        indirection;
    typedef mpl::list<type,indirection,cv> traits;
};

template<typename T>
struct unwrap_type<T*>
{
    typedef T type;
    typedef cv_unqualified cv;
    typedef pointer_        indirection;
    typedef mpl::list<type,indirection,cv> traits;
};

template<typename T>
struct unwrap_type<T const*>
{
    typedef T type;
    typedef const_ cv;
    typedef pointer_        indirection;
    typedef mpl::list<type,indirection,cv> traits;
};

template<typename T>
struct unwrap_type<T&>
{
    typedef T type;
    typedef cv_unqualified cv;
    typedef reference_        indirection;
    typedef mpl::list<type,indirection,cv> traits;
};

template<typename T>
struct unwrap_type<T const&>
{
    typedef T type;
    typedef const_ cv;
    typedef reference_        indirection;
    typedef mpl::list<type,indirection,cv> traits;
};

#define BOOST_CLIPP_UNWRAP_TYPE(T) \
boost::clipp::detail::unwrap_type<T>::traits()

template<typename indirection_tagT,typename cv_tagT>
struct wrap_decoration {
    template<typename T>
    struct inner {
        typedef T type;
    };
};

template<>
struct wrap_decoration<direct_,cv_unqualified>
{
    template<typename T>
    struct inner {
        typedef T type;
    };
};

template<>
struct wrap_decoration<direct_,const_>
{
    template<typename T>
    struct inner {
        typedef T& type;
    };
};

template<>
struct wrap_decoration<pointer_,cv_unqualified>
{
    template<typename T>
    struct inner {
        typedef T* type;
    };
};

template<>
struct wrap_decoration<pointer_,const_>
{
    template<typename T>
    struct inner {
        typedef T const* type;
    };
};

template<>
struct wrap_decoration<reference_,cv_unqualified>
{
    template<typename T>
    struct inner {
        typedef T& type;
    };
};

template<>
struct wrap_decoration<reference_,const_>
{
    template<typename T>
    struct inner {
        typedef T const& type;
    };
};


template<typename Traits>
struct result_traits
{
    BOOST_STATIC_CONSTANT(bool,direct_possible=(sizeof(Traits::type) <= sizeof(void*)));    
    typedef typename mpl::if_<
        mpl::and_<
            is_same<typename unwrap_indirection<Traits>::type,direct_>,
            mpl::bool_<direct_possible>
        >,
        Traits,
        mpl::list<typename unwrap_value_type<Traits>::type,reference_,const_>
    >::type traits;
    typedef typename unwrap_value_type<traits>::type type;
    typedef typename unwrap_indirection<traits>::type indirection;
    typedef typename unwrap_cv<traits>::type cv;
};  

template<typename Traits>
struct wrap_type {
    typedef typename unwrap_indirection<Traits>::type indirection;
    typedef typename unwrap_cv<Traits>::type cv;
    typedef typename unwrap_value_type<Traits>::type value_type;
    typedef typename mpl::if_<
        boost::is_same<cv,const_>,
        typename add_const<value_type>::type,
        value_type
    >::type cv_type;
    typedef typename mpl::if_<
        boost::is_same<indirection,pointer_>,
        typename add_pointer<cv_type>::type,
        typename mpl::if_<
            boost::is_same<indirection,reference_>,
            typename add_reference<cv_type>::type,
            cv_type
        >::type
    >::type type;
};

}}} //namespace boost::clipp::detail

#endif //BOOST_CLIPP_DETAIL_UNWRAP_TYPE_HPP_HOLT_31082003
