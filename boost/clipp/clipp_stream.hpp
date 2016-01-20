// Copyright Peder Holt 2003-2004. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_CLIPP_STREAM_HPP_HOLT_13022004
#define BOOST_CLIPP_CLIPP_STREAM_HPP_HOLT_13022004

#include <boost/clipp/context.hpp>
#include <ostream>

namespace boost { namespace clipp {

template<typename StreamT>
struct clipp_stream {
    clipp_stream(context* c,StreamT& stream) : context_(c) ,stream_(stream) {}
    clipp_stream(const clipp_stream& stream) : context_(stream.context_),stream_(stream.stream_) {}
    context* get_context() const {return context_;}
    std::ostream& get_stream() const {return stream_;}
private:
    context* context_;
    StreamT& stream_;
};

template<typename StreamT,typename T>
clipp_stream<StreamT>& operator<<(clipp_stream<StreamT>& stream,T arg)
{
    try {
        valueP input=wrap(arg,stream.get_context());
        stream.get_stream() << unwrap<const std::string&>(input["toString"]())();
    }
    catch(std::exception& e) {
        stream.get_stream() << e.what();
    }
    return stream;
}

template<typename StreamT>
clipp_stream<StreamT> wrap_stream(context* c,StreamT& stream) {
    return clipp_stream<StreamT>(c,stream);
}

}} //namespace boost::clipp

#endif //BOOST_CLIPP_CLIPP_STREAM_HPP_HOLT_13022004
