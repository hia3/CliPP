// Copyright Peder Holt 2003-2004. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_ARGUMENT_HPP_HOLT_29022004
#define BOOST_CLIPP_ARGUMENT_HPP_HOLT_29022004

#include <boost/mpl/int.hpp>
#include <boost/mpl/at.hpp>
#include <boost/clipp/unwrap.hpp>
#include <boost/clipp/detail/pre_wrap_value.hpp>

namespace boost { namespace clipp {

class arg {
public:
    arg(const char* name=NULL,const char* help_text=NULL) 
    {
        if(name) name_=name;
        if(help_text) help_text_=help_text;
    }
    template<typename T>
    arg& operator=(T const& arg) {
    	default_value_.set(arg);
        return *this;
    }
    valueP get_default_value(context* c) {
        return default_value_.get_value(c);
    }
    const std::string& name() {return name_;}
    const std::string& help_text() {return help_text_;}
    void signature(arg const& lhs) {
        default_value_=lhs.default_value_;
        name_=lhs.name_;
        help_text_=lhs.help_text_;
    }
private:
    detail::pre_wrap_value default_value_;
    std::string name_;
    std::string help_text_;
};

template<typename Signature,int Iarg>
class doc_arg : public arg
{
public:
    doc_arg() {}
    doc_arg(arg const& lhs) {
        signature(lhs);
    }
    std::string format(context* c) {
        typedef typename unwrap<BOOST_DEDUCED_TYPENAME mpl::at<Signature,mpl::int_<Iarg+4> >::type>::result_type result_type;
        std::string format = c->type_name(type_id<result_type>());
	    if(name().size()>0) format+= " " + name();
        if(valueP default_value=get_default_value(c)) {		
            format +=" = " + unwrap<std::string const&>(default_value["toString"]())();
        }
        return format;
    }
};


}} //namespace boost::clipp;

#endif //BOOST_CLIPP_ARGUMENT_HPP_HOLT_29022004
