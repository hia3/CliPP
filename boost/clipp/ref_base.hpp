// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_REF_BASE_HPP_HOLT_07092003
#define BOOST_CLIPP_REF_BASE_HPP_HOLT_07092003

#include <boost/clipp/config.hpp>
#include <boost/clipp/clipp_ptr.hpp>

namespace boost { namespace clipp {

class BOOST_CLIPP_EXPORT_IMPORT ref_base {
public:
    ref_base() : counter_(0) {}
    virtual ~ref_base() {}
    inline void add_ref() 
    {
        ++counter_;
    }
    inline void release() 
    {   
        --counter_;
        if(counter_<=0) destroy();
    }
    inline void destroy() 
    {
        delete this;
    }
private:
    int counter_;
};



} // namespace clipp;

inline void intrusive_ptr_add_ref(clipp::ref_base * p) 
{
    p->add_ref();
}
inline void intrusive_ptr_release(clipp::ref_base * p) 
{
    p->release();
}

} // namespace boost;

#endif //BOOST_CLIPP_REF_BASE_HPP_HOLT_07092003

