// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_VALUE_ITERATOR_HPP_HOLT_04122003
#define BOOST_CLIPP_VALUE_ITERATOR_HPP_HOLT_04122003

#include "boost/clipp/object.hpp"
#include <boost/call_traits.hpp>
#include <iterator>

namespace boost { namespace clipp {

template<typename T>struct is_pair : mpl::false_ {};
template<typename T,typename U>struct is_pair<std::pair<T,U> > : mpl::true_ {};
template<typename T,typename U>struct is_pair<std::pair<T,U>& > : mpl::true_ {};
template<typename T,typename U>struct is_pair<std::pair<T,U>const& > : mpl::true_ {};
template<typename T,typename U>struct is_pair<std::pair<T,U>* > : mpl::true_ {};
template<typename T,typename U>struct is_pair<std::pair<T,U>const* > : mpl::true_ {};


template<typename T, typename = void>
struct is_iterator : mpl::false_ {};
template<typename T>
struct is_iterator<T, typename std::enable_if<!std::is_same<typename std::iterator_traits<T>::value_type, void>::value>::type> : mpl::true_ {};
/*template<typename T>struct is_iterator : mpl::false_ {};
template<typename T,typename U,typename V> struct is_iterator<std::iterator<T,U,V> > : mpl::true_ {};*/

namespace detail {

struct iterator_holder_base 
{
    iterator_holder_base(iterator_holder_base const& rhs) : context_(rhs.context_) {}
    explicit iterator_holder_base(context* c) : context_(c) {}
    virtual ~iterator_holder_base() = 0 {}
    virtual void inc() = 0;
    virtual void dec() = 0;
    virtual valueP          get() const= 0;
    virtual bool            equals(iterator_holder_base const* rhs) const= 0;
    virtual iterator_holder_base* construct() = 0;
    context* context_;
};

template<typename IteratorT>
struct iterator_holder : iterator_holder_base
{
    static iterator_holder* create(IteratorT const& iterator,context* c) {return new iterator_holder(iterator,c);}
    iterator_holder(iterator_holder const& rhs) : iterator_holder_base(rhs) ,iterator_(rhs.iterator_) {}
    iterator_holder(IteratorT const& iterator,context* c) : iterator_(iterator) , iterator_holder_base(c) {}
    virtual void inc() {++iterator_;}
    virtual void dec() {--iterator_;}
    virtual valueP          get() const {return wrap_ref(*iterator_,*iterator_,context_);}
    virtual bool            equals(iterator_holder_base const* rhs) const {
        if(iterator_holder const * cmp = dynamic_cast<iterator_holder const *>(rhs)) {
            return cmp->iterator_==iterator_;
        }
        return false;
    }
    virtual iterator_holder_base* construct() {return new iterator_holder(*this);}
    IteratorT iterator_;
};
template<typename IteratorT>
struct pair_iterator_holder : iterator_holder_base
{
    pair_iterator_holder(pair_iterator_holder const& rhs) : iterator_holder_base(rhs) ,iterator_(rhs.iterator_) {}
    pair_iterator_holder(IteratorT const& iterator,context* c) : iterator_(iterator) , iterator_holder_base(c) {}
    virtual void inc() {++iterator_;}
    virtual void dec() {--iterator_;}
    virtual valueP          get() const {return wrap_ref(iterator_->second,iterator_->second,context_);}
    virtual bool            equals(iterator_holder_base const* rhs) const {
        if(pair_iterator_holder const * cmp = dynamic_cast<pair_iterator_holder const *>(rhs)) {
            return cmp->iterator_==iterator_;
        }
        return false;
    }
    virtual iterator_holder_base* construct() {return new pair_iterator_holder(*this);}
    IteratorT iterator_;
};
template<typename IteratorT>
struct iterator_holder_selector {
    static iterator_holder_base* create(IteratorT const& iterator,context* c) {
        typedef typename IteratorT::value_type value_type;
        typedef typename mpl::if_<
            is_pair<value_type>,
            pair_iterator_holder<IteratorT>,
            iterator_holder<IteratorT>
        >::type holder;
        return new holder(iterator,c);
    }
};


template<typename IteratorT>
iterator_holder_base* holder_selector(IteratorT iterator,context* c) {
    typedef typename mpl::if_<
        is_iterator<IteratorT>,
        iterator_holder_selector<IteratorT>,
        iterator_holder<IteratorT>
    >::type holder;
    return holder::create(iterator,c);
}

} //namespace detail

struct value_iterator : 
std::iterator<std::bidirectional_iterator_tag,valueP,valueP>
{

    
    template<typename IteratorT>
    value_iterator(IteratorT iterator,context* c) 
    :  m_holder(detail::holder_selector(iterator,c))
    {
    }
    value_iterator(value_iterator const& rhs) : m_holder(rhs.m_holder->construct()) 
    {}
    ~value_iterator() {}
    const valueP operator*() const {return m_holder->get();}
    valueP operator*()             {return m_holder->get();}
    value_iterator& operator++() {m_holder->inc();return *this;}
    value_iterator& operator--() {m_holder->dec();return *this;}
    value_iterator operator++(int) {
        value_iterator tmp(*this);
        m_holder->inc();
        return tmp;
    }
    value_iterator operator--(int) {
        value_iterator tmp(*this);
        m_holder->dec();
        return tmp;
    }

    friend bool operator==(value_iterator const& lhs,value_iterator const& rhs) {
        return lhs.m_holder->equals(rhs.m_holder.get());            
    }
    friend bool operator!=(value_iterator const& lhs,value_iterator const& rhs) {
        return !lhs.m_holder->equals(rhs.m_holder.get());
    }
protected:
    std::auto_ptr<detail::iterator_holder_base> m_holder;
};


}} // namespace boost::clipp

#endif //BOOST_CLIPP_VALUE_ITERATOR_HPP_HOLT_04122003
