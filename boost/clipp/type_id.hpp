// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.
#ifndef BOOST_CLIPP_TYPE_ID_HPP_HOLT_20082003
# define BOOST_CLIPP_TYPE_ID_HPP_HOLT_20082003

# include <boost/clipp/config.hpp>
# include <boost/clipp/detail/unwrap_type.hpp>
# include <boost/clipp/decoration.hpp>
# include <boost/operators.hpp>
# include <typeinfo>
# include <boost/static_assert.hpp>
# include <boost/type.hpp>

namespace boost { namespace clipp { 

struct type_detail : private totally_ordered<type_detail>
{
    inline type_detail(std::type_info const& = typeid(void),decoration d=d_direct);
    
    inline bool operator<(type_detail const& rhs) const;
    inline bool operator==(type_detail const& rhs) const;
    inline decoration get_decoration() const {return decoration_;}

    char const* name() const;

    std::type_info const* base_type_;
    decoration            decoration_;
};

template <class Traits>
inline type_detail type_id_traits(Traits)
{
    return type_detail(
        typeid(typename detail::unwrap_value_type<Traits>::type),
        decoration(detail::unwrap_decoration<Traits>::value)
    );
}

template<typename T>
struct type_id_struct {
    static type_detail do_() {
        return type_id_traits(BOOST_CLIPP_UNWRAP_TYPE(T));
    }
};

template<>
struct type_id_struct<void> {
    static type_detail do_() {
        return type_detail(typeid(void));
    }
};

template <class T>
inline type_detail type_id(boost::type<T>* dummy = NULL)
{
    return type_id_struct<T>::do_();
}

//
inline type_detail::type_detail(std::type_info const& id,decoration d)
    : base_type_(&id)
    , decoration_(d)
{
}

inline bool type_detail::operator<(type_detail const& rhs) const
{
    return base_type_->before(*rhs.base_type_)!=0;
}

inline bool type_detail::operator==(type_detail const& rhs) const
{
    return (*base_type_ == *rhs.base_type_)!=0;
}
  
inline char const* type_detail::name() const
{
    char const* raw_name = base_type_->name();
    return raw_name;
}

}} // namespace boost::clipp

#endif // BOOST_CLIPP_TYPE_ID_HPP_HOLT_20082003
