// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_READ_GENERATOR_EXPANDER_HPP_HOLT_15092003
#define BOOST_CLIPP_READ_GENERATOR_EXPANDER_HPP_HOLT_15092003

namespace boost { namespace clipp { namespace detail {

template<typename T>
struct read_generator {
    read_generator(T t,valueP* stored=nullptr) : t_(t), stored_(stored) {}
    typedef valueP result_type;

    template<typename cv_tagT>
    result_type execute(context* c,boost::type<T>,free_data_tag const&,cv_tagT) const
    {
        //Prevent garbage collection...
        if(stored_) {
            if(!*stored_) *stored_=wrap_struct<T>::wrap(t_,c);
        }
        return *stored_;
    }
    template<typename cv_tagT>
    result_type execute(context* c,boost::type<T>,reference_wrapper_tag const&,cv_tagT) const
    {
        return wrap_struct<T>::wrap(t_,c);
    }
    template<typename R,typename cv_tagT>
    result_type execute(context* c,boost::type<R>,free_function_tag const&,cv_tagT) const
    {
        return wrap_struct<R>::wrap(t_(),c);
    }
    template<typename R,typename cv_tagT,typename Signature>
    result_type execute(context* c,boost::type<R>,member_function_tag const& ct,cv_tagT,argument<Signature,0> const & a0) const
    {
        return wrap_struct<R>::wrap( (a0().*t_)(),c );
    }
    template<typename R,typename cv_tagT,typename Signature>
    result_type execute(context* c,boost::type<R>,member_data_tag const& ct,cv_tagT,argument<Signature,0> const& a0) const
    {
        return wrap_struct<R>::wrap(a0().*t_,c);
    }
    T t_;
    mutable valueP* stored_;
};

}}} //namespace boost::clipp::detail

#endif //BOOST_CLIPP_READ_GENERATOR_EXPANDER_HPP_HOLT_15092003
