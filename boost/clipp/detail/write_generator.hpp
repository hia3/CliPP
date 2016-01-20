// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_WRITE_GENERATOR_EXPANDER_HPP_HOLT_23102003
#define BOOST_CLIPP_WRITE_GENERATOR_EXPANDER_HPP_HOLT_23102003

namespace boost { namespace clipp { namespace detail {

template<typename T>
struct write_generator {
    write_generator(T t) : t_(t) {}
    typedef valueP result_type;

    template<typename R,typename cv_tagT,typename Signature>
    result_type execute(context* c,boost::type<R>,free_function_tag&,cv_tagT,argument<Signature,0>& a0) const
    {
        return wrap_struct<R>::wrap(t_(a0()),c);
    }
    template<typename R,typename cv_tagT,typename Signature>
    result_type execute(context* c,boost::type<R>,reference_wrapper_tag&,cv_tagT,argument<Signature,0>& a0) const
    {
        return wrap_struct<R>::wrap(t_=a0(),c);
    }
    template<typename cv_tagT,typename Signature>
    result_type execute(context* c,mpl::void_,free_function_tag&,cv_tagT,argument<Signature,0>& a0) const
    {
        t_(a0());
        return nullptr;
    }
    template<typename R,typename cv_tagT,typename Signature>
    result_type execute(context* c,boost::type<R>,member_function_tag& ct,cv_tagT,argument<Signature,0>& a0,argument<Signature,1>& a1) const
    {
        return wrap( (a0().*t_)(a1()),c );
    }
    template<typename cv_tagT,typename Signature>
    result_type execute(context* c,mpl::void_,member_function_tag& ct,cv_tagT,argument<Signature,0>& a0,argument<Signature,1>& a1) const
    {
        (a0().*t_)(a1());
        return nullptr;
    }
    template<typename R,typename cv_tagT,typename Signature>
    result_type execute(context* c,boost::type<R>,member_data_tag& ct,cv_tagT,argument<Signature,0>& a0,argument<Signature,1>& a1) const
    {
        return wrap<R>((a0().*t_)=a1(),c);
    }
    T t_;
};

}}} //namespace boost::clipp::detail

#endif //BOOST_CLIPP_READ_GENERATOR_EXPANDER_HPP_HOLT_15092003
