// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_DETAIL_DECORATED_CONVERTER_HPP_HOLT_07092003
#define BOOST_CLIPP_DETAIL_DECORATED_CONVERTER_HPP_HOLT_07092003

#include <boost/clipp/detail/converter.hpp>

namespace boost { namespace clipp { namespace detail {
    
class decorated_converter {
public:
    decorated_converter() {}
    void       add_converter(converterP c);
    converterP get_converter(decoration d) {return converters_[static_cast<unsigned>(d)];}
private:
    converterP converters_[6];
};

inline void decorated_converter::add_converter(converterP c)
{
    for(int i=0;i<6;++i) {
        if(!converters_[i]) converters_[i] = c->get_converter(decoration(i));
        else {
            converterP converter=c->get_converter(decoration(i));
            //Is this a better conversion than we already have? If so, use it
            if(converter && converter->get_precedence()>converters_[i]->get_precedence()) {
                converters_[i] = converter;
            }
        }
    }
}

}}} //namespace boost::clipp::detail

#endif //BOOST_CLIPP_DETAIL_DECORATED_CONVERTER_HPP_HOLT_07092003

