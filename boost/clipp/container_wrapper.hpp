// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_CONTAINER_WRAPPER_HPP_HOLT_06082003
#define BOOST_CLIPP_CONTAINER_WRAPPER_HPP_HOLT_06082003

#include "boost/clipp/container_base.hpp"

namespace boost { namespace clipp {

template<typename T>
struct unwrap;

template<class T>
class container_wrapper : public container_base  
{
public:
    typedef T container_type;
    typedef typename container_type::value_type value_type;

    ///construct empty container
    container_wrapper() {}

    ///copy constructor. Used to wrap existing containers, in the to_value converter
    container_wrapper(const container_type& container) : container_(container) {}
    container_wrapper(const container_base& container) {
        for(value_iterator it=container.begin();it!=container.end();++it) {
            container_.insert(
                        container_.end(),
                        unwrap<value_type>(*it)()
            );
        }
    }
    virtual ~container_wrapper() {}
    
    operator container_type&() {
        return container_;
    }
    operator container_type const&() const{
        return container_;
    }
    
    virtual value_iterator begin() const {
        return value_iterator(container_.begin(),get_context());
    }
    virtual value_iterator end() const {
        return value_iterator(container_.end(),get_context());
    }
    virtual size_t         size() const {
        return container_.size();
    }

    static void init(context* c);
private:
    container_type container_;
};

}} // namespace boost::clipp

#endif //BOOST_CLIPP_CONTAINER_WRAPPER_HPP_HOLT_06082003
