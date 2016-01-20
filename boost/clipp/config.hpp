// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_CONFIG_HPP_03082003
#   define BOOST_CLIPP_CONFIG_HPP_03082003

#   ifndef BOOST_CONFIG_HPP
#       include "boost/config.hpp"
#   endif

#   define BOOST_CLIPP_VERSION 500

#   ifdef BOOST_MSVC 
//'identifier' : identifier was truncated to 'number' characters in the debug information
#       pragma warning (disable:4786)
//'identifier' : class 'type' needs to have dll-interface to be used by clients of class 'type2'
#       pragma warning (disable:4251)
//'this' : used in base member initializer list
#       pragma warning (disable:4355) 
#       ifndef BOOST_CLIPP_EXPORTS
#           ifdef _DEBUG
#               define BOOST_CLIPP_LIB_NAME "boost_clippD.lib"
#           else
#               define BOOST_CLIPP_LIB_NAME "boost_clippR.lib"
#           endif
#           pragma comment(lib,BOOST_CLIPP_LIB_NAME)
#       endif

namespace boost {namespace clipp {namespace aux {
    template< typename T >
    struct msvc_never_true
    {
        enum { value = false };
    };
}}}

#define BOOST_CLIPP_IS_WRAPPED_TEMPLATE(wrapped)                                \
template <typename T>                                                           \
struct BOOST_PP_CAT(is_,wrapped)_helper                                         \
{                                                                               \
    static type_traits::no_type BOOST_TT_DECL _m_check(...);                    \
    template<typename U>                                                        \
    static type_traits::yes_type BOOST_TT_DECL _m_check(wrapped<U> const& (*)());\
    template<typename U>                                                        \
    static type_traits::yes_type BOOST_TT_DECL _m_check(wrapped<U>& (*)());\
                                                                                \
    typedef typename call_traits<T>::const_reference (*fn)();                            \
                                                                                \
    BOOST_STATIC_CONSTANT(bool, value =                                         \
        sizeof(_m_check(((fn)0)) ) == sizeof(type_traits::yes_type)             \
    );                                                                          \
};                                                                              \
template<typename T>                                                            \
struct BOOST_PP_CAT(is_,wrapped) : public                                       \
    mpl::bool_<BOOST_PP_CAT(is_,wrapped)_helper<T>::value>                      \
{};

#   define BOOST_PP_CHARIZE(text) #@text

#   else //BOOST_MSVC

#define BOOST_CLIPP_IS_WRAPPED_TEMPLATE(wrapped)                                \
template <typename T>                                                           \
struct BOOST_PP_CAT(is_,wrapped) : mpl::false_ {};                              \
template <typename T>                                                           \
struct BOOST_PP_CAT(is_,wrapped)<wrapped<T> > : mpl::true_ {};                 

#   endif

#if (defined(_WIN32) || defined(__CYGWIN__))
#  if defined(BOOST_CLIPP_EXPORTS)
#     define BOOST_CLIPP_EXPORT_IMPORT __declspec(dllexport)
#  else
#     define BOOST_CLIPP_EXPORT_IMPORT __declspec(dllimport)
#  endif
#endif

#endif // BOOST_CLIPP_CONFIG_HPP_03082003
