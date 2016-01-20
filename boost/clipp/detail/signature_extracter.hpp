// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_SIGNATURE_EXTRACTER_HPP_HOLT_30082003
#define BOOST_CLIPP_SIGNATURE_EXTRACTER_HPP_HOLT_30082003

#include <boost/clipp/detail/signature.hpp>
#include <boost/clipp/is_valarray.hpp>
#include <boost/clipp/error_messages.hpp>
#include <boost/clipp/unwrap.hpp>
#include <boost/mpl/minus.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/at.hpp>

namespace boost { namespace clipp { namespace detail {

/*template<class T>
constexpr unsigned signature_arity = mpl::minus<mpl::size<Signature>, mpl::int_<4> >::value;*/

template<typename Signature>
struct signature_arity
{
    BOOST_STATIC_CONSTANT(unsigned,value=(mpl::minus<mpl::size<Signature>,mpl::int_<4> >::value));
};

template<typename Signature>
using signature_type = typename mpl::at<Signature, mpl::int_<0> >::type;

//Return type selector
template<typename T>
struct return_type_selector
{
    typedef typename boost::type<T> type;
};

template<>
struct return_type_selector<void>
{
    typedef mpl::void_ type;
};

template<typename Signature>
using return_type_wrapper = typename return_type_selector<typename mpl::at<Signature, mpl::int_<1> >::type>::type;

template<typename Signature>
using return_type = typename mpl::at<Signature, mpl::int_<1> >::type;

template<typename Signature>
using data_type = typename mpl::at<Signature, mpl::int_<2> >::type;

//CV Tag type
template<typename Signature>
using cv_type = typename mpl::at<Signature, mpl::int_<3> >::type;

template<typename T>
class default_last_argument : public unwrap<T>
{
public:
    typedef typename unwrap<T>::result_type result_type;
    template<typename ArgumentListT>
    default_last_argument(const ArgumentListT& params,size_t nargs) : 
        unwrap<T>(params.size()>=nargs ? params[nargs-1] : nullptr) , params_size_(params.size()), nargs_(nargs) {}
    bool ok() const {
        if(params_size_!=nargs_) return false;
        return unwrap<T>::ok();
    }
    result_type operator()() const {
        if(params_size_!=nargs_) throw invalid_number_of_arguments(nargs_,params_size_);
        return unwrap<T>::operator()();
    }
    size_t params_size_;
    size_t nargs_;
};

template<class T>
class valarray_unwrapper
{
public:
    typedef T result_type;
    typedef typename unwrap_type<T>::type valarray_type;
    typedef typename valarray_type::value_type value_type;

    template<typename ArgumentListT>
    valarray_unwrapper(const ArgumentListT& params,size_t nargs) 
    : nargs_(nargs-1)
    , params_size_(params.size())
    {
        if(params_size_<nargs_) return;
        for(size_t i=nargs_;i<params.size();++i) {
            params_.push_back(params[i]);
        }
    }
    bool ok() const
    {
        if(params_size_ < nargs_) return false;
        for(size_t i=0;i<params_.size();++i) {
            if(!params_[i].ok()) return false;
        }
        return true;
    }
    result_type operator ()() const {
        internal_unwrap();
        return optional_dereference(is_pointer<T>());
    }
private:
    void internal_unwrap() const {
        int size=params_size_-nargs_;
        if(size<0) throw invalid_number_of_arguments(nargs_,params_size_,true);
        if(size==0) return;
        //Already unwrapped once.
        if(size==container_.size()) return;
        container_.resize(size);
        for(size_t i=0;i<params_.size();++i) {
            container_[i] = params_[i]();
        }
    }

    valarray_type* optional_dereference(mpl::bool_<true>) const {
        return &container_;
    }
    valarray_type& optional_dereference(mpl::bool_<false>) const {
        return container_;
    }

    size_t nargs_;
    size_t params_size_;
    std::vector<unwrap<value_type> > params_;
    mutable valarray_type container_;
};

template<typename T>
struct last_argument_selector
{
    typedef typename mpl::if_<
        is_valarray<T>,
        valarray_unwrapper<T>,
        default_last_argument<T>
    >::type type;
};

template<bool>
struct argument_selector {
    template<typename Signature,int IArg>
    struct inner : unwrap<typename mpl::at<Signature,typename mpl::int_<IArg+4> >::type> 
    {
        typedef unwrap<typename mpl::at<Signature,typename mpl::int_<IArg+4> >::type> base_type;
        template<typename ArgumentListT>
        inner(const ArgumentListT& params) : base_type(params.size()>IArg ? params[IArg] : nullptr) {}
    };
};

//Last argument
template<>
struct argument_selector<true>
{
    template<typename Signature,int IArg>
    struct inner : last_argument_selector<typename mpl::at<Signature,typename mpl::int_<IArg+4> >::type>::type
    {
        typedef typename last_argument_selector<typename mpl::at<Signature,typename mpl::int_<IArg+4> >::type>::type base_type;
        template<typename ArgumentListT>
        inner(const ArgumentListT& params) : base_type(params,signature_arity<Signature>::value) {};
    };
};

#if BOOST_WORKAROUND( BOOST_MSVC, <= 1300)

//Argument extraction
template<typename Signature,int IArg>
struct argument : argument_selector<signature_arity<Signature>::value == IArg+1>::inner<Signature,IArg>
{
    typedef typename argument_selector<signature_arity<Signature>::value == IArg+1>::inner<Signature,IArg> base_type;
    template<typename ArgumentListT>
    argument(const ArgumentListT& params) : base_type(params) {}
};

#else

template<typename Signature,int IArg>
struct select_argument_selector {
    typedef typename mpl::if_<
        mpl::bool_<signature_arity<Signature>::value == IArg+1>,
        argument_selector<true>,
        argument_selector<false>
    >::type selector;
    typedef typename selector::template inner<Signature,IArg> type;
};

//Argument extraction
template<typename Signature,int IArg>
struct argument : select_argument_selector<Signature,IArg>::type//argument_selector<signature_arity<Signature>::value == IArg+1>::inner<Signature,IArg>
{
    typedef typename select_argument_selector<Signature,IArg>::type base_type;//argument_selector<signature_arity<Signature>::value == IArg+1>::inner<Signature,IArg> base_type;
    template<typename ArgumentListT>
    argument(const ArgumentListT& params) : base_type(params) {}
};

#endif

}}} //namespace boost::clipp::detail

#endif //BOOST_CLIPP_SIGNATURE_EXTRACTER_HPP_HOLT_30082003

