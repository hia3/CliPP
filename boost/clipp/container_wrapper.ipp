#ifndef BOOST_CLIPP_CONTAINER_WRAPPER_IPP_HOLT_29112003
#define BOOST_CLIPP_CONTAINER_WRAPPER_IPP_HOLT_29112003

namespace boost { namespace clipp {

template<class T>
inline void container_wrapper<T>::init(context* c)
{
    class_<container_wrapper<T>,container_base,container_type> cls("container_wrapper",c);

    cls.converter(arguments<const container_base&>());
    detail::register_traits(BOOST_CLIPP_UNWRAP_TYPE(value_type),c);
}

}} // namespace boost::clipp

#endif

