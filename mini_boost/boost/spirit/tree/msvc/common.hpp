/*=============================================================================
    Spirit v1.6.2
    Copyright (c) 2001-2003 Daniel Nuffer
    Copyright (c) 2004      Peder Holt
    http://spirit.sourceforge.net/

    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at 
    http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef BOOST_SPIRIT_TREE_MSVC_COMMON_HPP
#define BOOST_SPIRIT_TREE_MSVC_COMMON_HPP

#include <vector>

#if defined(BOOST_NO_TEMPLATED_ITERATOR_CONSTRUCTORS)
#include <algorithm>
#endif

#include <boost/ref.hpp>
#include <boost/call_traits.hpp>

#include <boost/spirit/core.hpp>

#if defined(BOOST_MSVC) && (BOOST_MSVC <= 1300)
#define BOOST_SPIRIT_MP_TYPE_COMPUTER_ARGS typename T, typename Pizza = nil_t
#else
#define BOOST_SPIRIT_MP_TYPE_COMPUTER_ARGS typename T
#endif

#if defined(BOOST_SPIRIT_DEBUG) && \
    (BOOST_SPIRIT_DEBUG_FLAGS_NODES & BOOST_SPIRIT_DEBUG_FLAGS_TREES)
#include <iostream>
#endif

#if defined(BOOST_NO_STD_ITERATOR_TRAITS)
#define BOOST_SPIRIT_IT_NS impl
#else
#define BOOST_SPIRIT_IT_NS std
#endif

///////////////////////////////////////////////////////////////////////////////
namespace boost { namespace spirit {


#if BOOST_WORKAROUND( BOOST_MSVC, <= 1300)
                                                             
BOOST_SPIRIT_DEPENDENT_TEMPLATE_WRAPPER(node_factory_wrapper, factory);

template <typename NodeFactoryT, typename IteratorT>
struct node_factory
{
    typedef typename impl::node_factory_wrapper<NodeFactoryT>
        ::template result_<IteratorT>::param_t type;
};

BOOST_SPIRIT_DEPENDENT_TEMPLATE_WRAPPER2(directive_parser_wrapper, action_def);

template <typename ActionParserT, typename ParserT, typename ActionT>
struct directive_parser 
{
    typedef typename impl::directive_parser_wrapper<ActionParserT>
        ::template result_<ParserT,ActionT>::param_t type;
};

#else

template <typename NodeFactoryT, typename IteratorT>
struct node_factory
{
    typedef typename NodeFactoryT::template factory<IteratorT> type;
};

template <typename ActionParserT, typename ParserT, typename ActionT>
struct directive_parser 
{
    typedef typename ActionParserT::template action_def<ParserT, ActionT> type;
};

#endif

///////////////////////////////////////////////////////////////////////////////
template <typename T>
struct tree_node;

#if BOOST_WORKAROUND( BOOST_MSVC, <= 1300)

template <typename IteratorT, typename ValueT>
struct node_iter_data;

#else 

template <typename IteratorT = char const*, typename ValueT = nil_t>
struct node_iter_data;

template <typename T>
void swap(tree_node<T>& a, tree_node<T>& b);

template <typename T, typename V>
void swap(node_iter_data<T, V>& a, node_iter_data<T, V>& b);

#endif


namespace impl {
    template <typename T>
    inline void cp_swap(T& t1, T& t2);
}

template <typename T>
struct tree_node
{
    typedef T parse_node_t;
    typedef std::vector<tree_node<T> > children_t;
    typedef typename children_t::iterator tree_iterator;
    typedef typename children_t::const_iterator const_tree_iterator;

    T value;
    children_t children;

    tree_node()
        : value()
        , children()
    {}

    explicit tree_node(T const& v)
        : value(v)
        , children()
    {}

    tree_node(T const& v, children_t const& c)
        : value(v)
        , children(c)
    {}

    tree_node(tree_node const& rhs) 
        : value(rhs.value)
        , children(rhs.children)
    {
    }

    void swap(tree_node<T>& x)
    {
        impl::cp_swap(value, x.value);
        impl::cp_swap(children, x.children);
    }

// Intel V5.0.1 has a problem without this explicit operator=
    tree_node &operator= (tree_node const &rhs)
    {
        tree_node(rhs).swap(*this);
        return *this;
    }

};

#if defined(BOOST_SPIRIT_DEBUG) && \
    (BOOST_SPIRIT_DEBUG_FLAGS_NODES & BOOST_SPIRIT_DEBUG_FLAGS_TREES)
template <typename T>
inline std::ostream&
operator<<(std::ostream& o, tree_node<T> const& n)
{
    static int depth = 0;
    o << "\n";
    for (int i = 0; i <= depth; ++i)
    {
        o << "\t";
    }
    o << "(depth = " << depth++ << " value = " << n.value;
    int c = 0;
    for (typename tree_node<T>::children_t::const_iterator it = n.children.begin();
         it != n.children.end(); ++it)
    {
        o << " children[" << c++ << "] = " << *it;
    }
    o << ")";
    --depth;
    return o;
}
#endif

//////////////////////////////////
template <typename IteratorT, typename ValueT>
struct node_iter_data
{
    typedef IteratorT iterator_t;
    typedef IteratorT /*const*/ const_iterator_t;

    node_iter_data()
        : first(), last(), is_root_(false), parser_id_(), value_()
        {}

    node_iter_data(IteratorT const& _first, IteratorT const& _last)
        : first(_first), last(_last), is_root_(false), parser_id_(), value_()
        {}
    node_iter_data(node_iter_data const& rhs)
        : first(rhs.first)
        , last(rhs.last)
        , is_root_(rhs.is_root_)
        , parser_id_(rhs.parser_id_)
        , value_(rhs.value_)
        {}

    void swap(node_iter_data& x)
    {
        impl::cp_swap(first, x.first);
        impl::cp_swap(last, x.last);
        impl::cp_swap(parser_id_, x.parser_id_);
        impl::cp_swap(is_root_, x.is_root_);
        impl::cp_swap(value_, x.value_);
    }

    IteratorT begin()
    {
        return first;
    }

    IteratorT const& begin() const
    {
        return first;
    }

    IteratorT end()
    {
        return last;
    }

    IteratorT const& end() const
    {
        return last;
    }

    bool is_root() const
    {
        return is_root_;
    }

    void is_root(bool b)
    {
        is_root_ = b;
    }

    parser_id id() const
    {
        return parser_id_;
    }

    void id(parser_id r)
    {
        parser_id_ = r;
    }

    ValueT const& value() const
    {
        return value_;
    }

    void value(ValueT const& v)
    {
        value_ = v;
    }

private:
    IteratorT first, last;
    bool is_root_;
    parser_id parser_id_;
    ValueT value_;

public:
};

#if defined(BOOST_SPIRIT_DEBUG) && \
    (BOOST_SPIRIT_DEBUG_FLAGS_NODES & BOOST_SPIRIT_DEBUG_FLAGS_TREES)
// value is default nil_t, so provide an operator<< for nil_t
inline std::ostream&
operator<<(std::ostream& o, nil_t const&)
{
    return o;
}

template <typename IteratorT, typename ValueT>
inline std::ostream&
operator<<(std::ostream& o, node_iter_data<IteratorT, ValueT> const& n)
{
    o << "(id = " << n.id() << " text = \"";
    typedef
        typename BOOST_SPIRIT_IT_NS::iterator_traits<IteratorT>::value_type
        iterator_t;
    std::copy(n.begin(), n.end(), std::ostream_iterator<iterator_t>(o));
    o << "\" is_root = " << n.is_root()
        << " value = " << n.value() << ")";
    return o;
}
#endif

//////////////////////////////////
template <typename IteratorT = char const*, typename ValueT = nil_t>
struct node_val_data
{
    typedef
        typename BOOST_SPIRIT_IT_NS::iterator_traits<IteratorT>::value_type
        value_type;
    typedef std::vector<value_type> container_t;
    typedef typename container_t::iterator iterator_t;
    typedef typename container_t::const_iterator const_iterator_t;

    node_val_data()
        : text(), is_root_(false), parser_id_(), value_()
        {}

#if defined(BOOST_NO_TEMPLATED_ITERATOR_CONSTRUCTORS)
    // This constructor is for building text out of vector iterators
    template <typename IteratorT2>
    node_val_data(IteratorT2 const& _first, IteratorT2 const& _last)
        : text(), is_root_(false), parser_id_(), value_()
        {
            std::copy(_first, _last, std::inserter(text, text.end()));
        }
    //This constructor must come after the templatified constructor, of MSVC will have problems
    node_val_data(IteratorT const& _first, IteratorT const& _last)
        : text(), is_root_(false), parser_id_(), value_()
        {
            std::copy(_first, _last, std::inserter(text, text.end()));
        }

#else
    node_val_data(IteratorT const& _first, IteratorT const& _last)
        : text(_first, _last), is_root_(false), parser_id_(), value_()
        {}

    // This constructor is for building text out of vector iterators
    template <typename IteratorT2>
    node_val_data(IteratorT2 const& _first, IteratorT2 const& _last)
        : text(_first, _last), is_root_(false), parser_id_(), value_()
        {}
#endif

    void swap(node_val_data& x)
    {
        impl::cp_swap(text, x.text);
        impl::cp_swap(is_root_, x.is_root_);
        impl::cp_swap(parser_id_, x.parser_id_);
        impl::cp_swap(value_, x.value_);
    }

    typename container_t::iterator begin()
    {
        return text.begin();
    }

    typename container_t::const_iterator begin() const
    {
        return text.begin();
    }

    typename container_t::iterator end()
    {
        return text.end();
    }

    typename container_t::const_iterator end() const
    {
        return text.end();
    }

    bool is_root() const
    {
        return is_root_;
    }

    void is_root(bool b)
    {
        is_root_ = b;
    }

    parser_id id() const
    {
        return parser_id_;
    }

    void id(parser_id r)
    {
        parser_id_ = r;
    }

    ValueT const& value() const
    {
        return value_;
    }

    void value(ValueT const& v)
    {
        value_ = v;
    }

private:
    container_t text;
    bool is_root_;
    parser_id parser_id_;
    ValueT value_;
};

#if defined(BOOST_SPIRIT_DEBUG) && \
    (BOOST_SPIRIT_DEBUG_FLAGS_NODES & BOOST_SPIRIT_DEBUG_FLAGS_TREES)
template <typename IteratorT, typename ValueT>
inline std::ostream&
operator<<(std::ostream& o, node_val_data<IteratorT, ValueT> const& n)
{
    o << "(id = " << n.id() << " text = \"";
    typedef
        typename BOOST_SPIRIT_IT_NS::iterator_traits<IteratorT>::value_type
        iterator_t;
    std::copy(n.begin(), n.end(), std::ostream_iterator<iterator_t>(o));
    o << "\" is_root = " << n.is_root()
        << " value = " << n.value() << ")";
    return o;
}
#endif

#if! BOOST_WORKAROUND( BOOST_MSVC, <= 1300)

template <typename T>
inline void 
swap(tree_node<T>& a, tree_node<T>& b)
{
    a.swap(b);
}

template <typename T, typename V>
inline void 
swap(node_iter_data<T, V>& a, node_iter_data<T, V>& b)
{
    a.swap(b);
}

#endif

//////////////////////////////////
template <typename ValueT = nil_t>
class node_iter_data_factory;

//////////////////////////////////
template <typename ValueT>
class node_iter_data_factory
{
public:
    // This inner class is so that node_iter_data_factory can simluate
    // a template template parameter
    typedef ValueT attr_t;

    template <typename IteratorT>
    class factory
    {
    public:
        typedef IteratorT iterator_t;
        typedef node_iter_data<iterator_t, ValueT> node_t;
        typedef ValueT attr_t;

        static node_t create_node(iterator_t const& first, iterator_t const& last,
                bool /*is_leaf_node*/)
        {
            return node_t(first, last);
        }

        static node_t empty_node()
        {
            return node_t();
        }

        // precondition: ContainerT contains a tree_node<node_t>.  And all
        // iterators in the container point to the same sequence.
        template <typename ContainerT>
        static node_t group_nodes(ContainerT const& nodes)
        {
            return node_t(nodes.begin()->value.begin(),
                    nodes.back().value.end());
        }
    };
};


//////////////////////////////////
template <typename ValueT>
class node_val_data_factory
{
public:
    // This inner class is so that node_val_data_factory can simluate
    // a template template parameter
    typedef ValueT attr_t;

    template <typename IteratorT>
    class factory
    {
    public:
        typedef IteratorT iterator_t;
        typedef node_val_data<iterator_t, ValueT> node_t;
        typedef ValueT attr_t;

        static node_t create_node(iterator_t const& first, iterator_t const& last,
                bool is_leaf_node)
        {
            if (is_leaf_node)
                return node_t(first, last);
            else
                return node_t();
        }

        static node_t empty_node()
        {
            return node_t();
        }

        template <typename ContainerT>
        static node_t group_nodes(ContainerT const& nodes)
        {
            typename node_t::container_t c;
            // copy all the nodes text into a new one
            for (typename ContainerT::const_iterator i = nodes.begin();
                    i != nodes.end(); ++i)
            {
                // See docs: token_node_d or leaf_node_d cannot be used with a
                // rule inside the [].
                assert(i->children.size() == 0);
                c.insert(c.end(), i->value.begin(), i->value.end());
            }
            return node_t(c.begin(), c.end());
        }
    };
};


//////////////////////////////////
template <typename ValueT = nil_t>
class node_all_val_data_factory;

//////////////////////////////////
template <typename ValueT>
class node_all_val_data_factory
{
public:
    // This inner class is so that node_all_val_data_factory can simluate
    // a template template parameter
    typedef ValueT attr_t;

    template <typename IteratorT>
    class factory
    {
    public:
        typedef IteratorT iterator_t;
        typedef node_val_data<iterator_t, ValueT> node_t;
        typedef ValueT attr_t;

        static node_t create_node(iterator_t const& first, iterator_t const& last,
                bool /*is_leaf_node*/)
        {
            return node_t(first, last);
        }

        static node_t empty_node()
        {
            return node_t();
        }

        template <typename ContainerT>
        static node_t group_nodes(ContainerT const& nodes)
        {
            typename node_t::container_t c;
            // copy all the nodes text into a new one
            for (typename ContainerT::const_iterator i = nodes.begin();
                    i != nodes.end(); ++i)
            {
                // See docs: token_node_d or leaf_node_d cannot be used with a
                // rule inside the [].
                assert(i->children.size() == 0);
                c.insert(c.end(), i->value.begin(), i->value.end());
            }
            return node_t(c.begin(), c.end());
        }
    };
};

///////////////////////////////////////////////////////////////////////////////
//  forward declaration
template <
    typename IteratorT,
    typename NodeFactoryT = node_val_data_factory<nil_t>,
    typename T = nil_t
>
class tree_match;

namespace impl {
//    template <typename T>
//    struct tree_match_attr
//    {
//        template <typename MatchT>
//        static T get(MatchT const& m)
//        { return T(m.value()); }
//
//        template <typename IteratorT, typename NodeFactoryT>
//        static T get(tree_match<IteratorT, NodeFactoryT, nil_t> const& /*m*/)
//        { return T(); }
//
//        static T get_default()
//        { return T(); }
//    };
//
//    //////////////////////////////////
//    template <typename T>
//    struct tree_match_attr<boost::reference_wrapper<T> >
//    {
//        template <typename MatchT>
//        static boost::reference_wrapper<T>
//        get(MatchT const& m)
//        { return boost::reference_wrapper<T>(m.value()); }
//
//        template <typename IteratorT, typename NodeFactoryT>
//        static boost::reference_wrapper<T>
//        get(tree_match<IteratorT, NodeFactoryT, nil_t> const& /*m*/)
//        {
//            static T v;
//            return boost::reference_wrapper<T>(v);
//        }
//
//        static boost::reference_wrapper<T>
//        get_default()
//        {
//            static T v;
//            return boost::reference_wrapper<T>(v);
//        }
//    };
//
//    //////////////////////////////////
//    template <>
//    struct tree_match_attr<nil_t>
//    {
//        template <typename MatchT>
//        static nil_t get(MatchT const& /*m*/)
//        { return nil_t(); }
//        static nil_t get_default()
//        { return nil_t(); }
//    };

    // can't call unqualified swap from within classname::swap
    // as Koenig lookup rules will find only the classname::swap
    // member function not the global declaration, so use cp_swap
    // as a forwarding function (JM):
#if __GNUC__ == 2
    using ::std::swap;
#endif
    template <typename T>
    inline void cp_swap(T& t1, T& t2)
    {
        using std::swap;
        using boost::swap;
        #if! BOOST_WORKAROUND( BOOST_MSVC, <= 1300)
        using boost::spirit::swap;
        #endif
        swap(t1, t2);
    }
}

//////////////////////////////////
template <typename IteratorT, typename NodeFactoryT, typename T>
class tree_match : public match<T>
{
public:

    typedef typename node_factory<NodeFactoryT,IteratorT>::type node_factory_t;
    typedef typename node_factory_t::node_t parse_node_t;
    typedef tree_node<parse_node_t> node_t;
    typedef typename node_t::children_t container_t;
    typedef typename container_t::iterator tree_iterator;
    typedef typename container_t::const_iterator const_tree_iterator;

    typedef T attr_t;
    typedef typename boost::call_traits<T>::param_type      param_type;
    typedef typename boost::call_traits<T>::reference       reference;
    typedef typename boost::call_traits<T>::const_reference const_reference;

    tree_match()
    : match<T>(), trees()
    {}

    explicit
    tree_match(std::size_t length)
    : match<T>(length), trees()
    {}

    tree_match(std::size_t length, parse_node_t const& n)
    : match<T>(length), trees()
    { trees.push_back(node_t(n)); }

    tree_match(std::size_t length, param_type val, parse_node_t const& n)
    : match<T>(length, val), trees()
    { trees.push_back(node_t(n)); }

    template <typename T2>
    tree_match(match<T2> const& other)
    : match<T>(other), trees()
    {}

    template <typename T2, typename T3, typename T4>
    tree_match(tree_match<T2, T3, T4> const& other)
    : match<T>(other), trees()
    { impl::cp_swap(trees, other.trees); }

    template <typename T2>
    tree_match&
    operator=(match<T2> const& other)
    {
        match<T>::operator=(other);
        return *this;
    }

    template <typename T2, typename T3, typename T4>
    tree_match&
    operator=(tree_match<T2, T3, T4> const& other)
    {
        match<T>::operator=(other);
        impl::cp_swap(trees, other.trees);
        return *this;
    }

    tree_match(tree_match const& x)
    : match<T>(x), trees()
    {
        // use auto_ptr like ownership for the trees data member
        impl::cp_swap(trees, x.trees);
    }

    tree_match& operator=(tree_match const& x)
    {
        tree_match tmp(x);
        this->swap(tmp);
        return *this;
    }

    void swap(tree_match& x)
    {
        int l=length();
        T attr=value();
        match<T>::operator=(x);
        x.match<T>::operator=(match<T>(l,attr));
        impl::cp_swap(trees, x.trees);
    }

    mutable container_t trees;
};

#if defined(BOOST_SPIRIT_DEBUG) && \
    (BOOST_SPIRIT_DEBUG_FLAGS_NODES & BOOST_SPIRIT_DEBUG_FLAGS_TREES)
template <typename IteratorT, typename NodeFactoryT, typename T>
inline std::ostream&
operator<<(std::ostream& o, tree_match<IteratorT, NodeFactoryT, T> const& m)
{
    typedef
        typename tree_match<IteratorT, NodeFactoryT, T>::container_t::iterator
        iterator;

    o << "(length = " << m.length();
    int c = 0;
    for (iterator i = m.trees.begin(); i != m.trees.end(); ++i)
    {
        o << " trees[" << c++ << "] = " << *i;
    }
    o << ")";
    return o;
}
#endif

//////////////////////////////////
struct tree_policy
{
    template <typename FunctorT, typename MatchT>
    static void apply_op_to_match(FunctorT const& /*op*/, MatchT& /*m*/)
    {}

    template <typename MatchT, typename Iterator1T, typename Iterator2T>
    static void group_match(MatchT& /*m*/, parser_id const& /*id*/,
            Iterator1T const& /*first*/, Iterator2T const& /*last*/)
    {}

    template <typename MatchT>
    static void concat(MatchT& /*a*/, MatchT const& /*b*/)
    {}
};

//////////////////////////////////
template <
    typename MatchPolicyT,
    typename IteratorT,
    typename NodeFactoryT,
    typename TreePolicyT
>
struct common_tree_match_policy
{
    template <BOOST_SPIRIT_MP_TYPE_COMPUTER_ARGS>
    struct result { typedef tree_match<IteratorT, NodeFactoryT, T> type; };

    typedef tree_match<IteratorT, NodeFactoryT> match_t;
    typedef IteratorT iterator_t;
    typedef TreePolicyT tree_policy_t;
    typedef NodeFactoryT factory_t;

    static const match_t no_match() { return match_t(); }
    static const match_t empty_match()
    { return match_t(0, tree_policy_t::empty_node()); }

    template <typename AttrT, typename Iterator1T, typename Iterator2T>
    static tree_match<IteratorT, NodeFactoryT, AttrT> create_match(
        std::size_t length,
        AttrT const& val,
        Iterator1T const& first,
        Iterator2T const& last)
    {
        typedef NodeFactoryT::attr_t attr_t;
#if defined(BOOST_SPIRIT_DEBUG) && \
    (BOOST_SPIRIT_DEBUG_FLAGS_NODES & BOOST_SPIRIT_DEBUG_FLAGS_TREES)
        BOOST_SPIRIT_DEBUG_OUT << "create_node.  creating node"
            " text: \"";
        for (Iterator1T it = first; it != last; ++it)
            BOOST_SPIRIT_DEBUG_OUT << *it;
        BOOST_SPIRIT_DEBUG_OUT << "\"" << std::endl;
#endif
        return tree_match<IteratorT, NodeFactoryT, AttrT>(length, val,
            tree_policy_t::create_node(length, first, last, val, true, is_convertible<AttrT,attr_t>()));
    }

    template <typename Match1T, typename Match2T>
    static void concat_match(Match1T& a, Match2T const& b)
    {
        BOOST_SPIRIT_ASSERT(a && b);
        if (a.length() == 0)
        {
            a = b;
            return;
        }
        else if (b.length() == 0)
        {
            return;
        }
        a.concat(b);
        tree_policy_t::concat(a, b);
    }

    template <typename MatchT, typename IteratorT2>
    void
    group_match(
        MatchT&             m,
        parser_id const&    id,
        IteratorT2 const&   first,
        IteratorT2 const&   last) const
    {
        tree_policy_t::group_match(m, id, first, last);
    }
};

//////////////////////////////////
template <typename MatchPolicyT, typename NodeFactoryT>
struct common_tree_tree_policy
{
    typedef typename MatchPolicyT::iterator_t iterator_t;
    typedef typename MatchPolicyT::match_t match_t;
    typedef typename node_factory<NodeFactoryT,iterator_t>::type factory_t;
    typedef typename factory_t::node_t node_t;

    template <typename Iterator1T, typename Iterator2T,typename ValueT>
        static node_t
        create_node(std::size_t /*length*/, Iterator1T const& first, 
            Iterator2T const& last, ValueT const& val, bool leaf_node, mpl::bool_<true>)
    {
        node_t node = factory_t::create_node(first, last, leaf_node);
        node.value(val);
        return node;
    }
    template <typename Iterator1T, typename Iterator2T,typename ValueT>
        static node_t
        create_node(std::size_t /*length*/, Iterator1T const& first, 
            Iterator2T const& last, ValueT const& val, bool leaf_node, mpl::bool_<false>)
    {
        return factory_t::create_node(first, last, leaf_node);
    }

    static node_t
        empty_node()
    {
        return factory_t::empty_node();
    }

    template <typename FunctorT,typename MatchT>
        static void apply_op_to_match(FunctorT const& op, MatchT& m)
    {
        op(m);
    }
};

//////////////////////////////////
// directives to modify how the parse tree is generated

struct no_tree_gen_node_parser_gen;

template <typename T>
struct no_tree_gen_node_parser
:   public unary<T, parser<no_tree_gen_node_parser<T> > >
{
    typedef no_tree_gen_node_parser<T> self_t;
    typedef no_tree_gen_node_parser_gen parser_generator_t;
    typedef unary_parser_category parser_category_t;
//    typedef no_tree_gen_node_parser<T> const &embed_t;

    no_tree_gen_node_parser(T const& a)
    : unary<T, parser<no_tree_gen_node_parser<T> > >(a) {}

    template <typename ScannerT>
    typename parser_result<self_t, ScannerT>::type
    parse(ScannerT const& scanner) const
    {
        typedef typename ScannerT::iteration_policy_t iteration_policy_t;
        typedef match_policy match_policy_t;
        typedef typename ScannerT::action_policy_t action_policy_t;
        typedef scanner_policies<
            iteration_policy_t,
            match_policy_t,
            action_policy_t
        > policies_t;

        typedef ScannerT::rebind_policies<policies_t>::type scanner_t;
        scanner_t rebound_scanner=scanner.change_policies(policies_t());
        return this->subject().parse(rebound_scanner);
    }
};

//////////////////////////////////
struct no_tree_gen_node_parser_gen
{
    template <typename T>
    struct result {

        typedef no_tree_gen_node_parser<T> type;
    };

    template <typename T>
    static no_tree_gen_node_parser<T>
    generate(parser<T> const& s)
    {
        return no_tree_gen_node_parser<T>(s.derived());
    }

    template <typename T>
    no_tree_gen_node_parser<T>
    operator[](parser<T> const& s) const
    {
        return no_tree_gen_node_parser<T>(s.derived());
    }
};

//////////////////////////////////
const no_tree_gen_node_parser_gen no_node_d = no_tree_gen_node_parser_gen();

namespace impl {

#if BOOST_WORKAROUND(BOOST_MSVC , <= 1300)

using ::boost::type_traits::yes_type;
using ::boost::type_traits::no_type;

template <typename MatchPolicyT, typename IteratorT, typename NodeFactoryT, typename TreePolicyT>
yes_type  tree_policy_selector_checker(common_tree_match_policy<MatchPolicyT, IteratorT, NodeFactoryT, TreePolicyT>*);
no_type tree_policy_selector_checker(...);

template <typename MatchPolicyT>
struct tree_policy_selector
{
    BOOST_STATIC_CONSTANT(bool, value = sizeof(tree_policy_selector_checker((MatchPolicyT*)0)) == sizeof(yes_type));

    template<bool tree_policy> struct inner;
    template<> 
    struct inner<true>
    {
        typedef MatchPolicyT::tree_policy_t type;
    };
    template<> 
    struct inner<false>
    {
        typedef spirit::tree_policy type;
    };

    typedef inner<value>::type type;
};

#else 

template <typename MatchPolicyT>
struct tree_policy_selector
{
    typedef tree_policy type;
};

#endif

} // namespace impl

//////////////////////////////////
template <typename NodeParserT>
struct node_parser_gen;

template <typename T, typename NodeParserT>
struct node_parser
:   public unary<T, parser<node_parser<T, NodeParserT> > >
{
    typedef node_parser<T, NodeParserT> self_t;
    typedef node_parser_gen<NodeParserT> parser_generator_t;
    typedef unary_parser_category parser_category_t;
//    typedef node_parser<T, NodeParserT> const &embed_t;

    node_parser(T const& a=T())
    : unary<T, parser<node_parser<T, NodeParserT> > >(a) {}

    template <typename ScannerT>
    struct result
    {
        typedef typename parser_result<T,ScannerT>::type type;
    };

    template <typename ScannerT>
    typename parser_result<self_t, ScannerT>::type
    parse(ScannerT const& scanner) const
    {
        typename parser_result<T, ScannerT>::type hit = this->subject().parse(scanner);
        if (hit)
        {
            impl::tree_policy_selector<BOOST_DEDUCED_TYPENAME ScannerT::match_policy_t>::type::apply_op_to_match(NodeParserT(), hit);
        }
        return hit;
    }
};

//////////////////////////////////
template <typename NodeParserT>
struct node_parser_gen
{
    template <typename T>
    struct result {

        typedef node_parser<T, NodeParserT> type;
    };

    template <typename T>
    static node_parser<T, NodeParserT>
    generate(parser<T> const& s)
    {
        return node_parser<T, NodeParserT>(s.derived());
    }

    template <typename T>
    node_parser<T, NodeParserT>
    operator[](parser<T> const& s) const
    {
        return node_parser<T, NodeParserT>(s.derived());
    }
};

struct discard_node_op
{
    template <typename MatchT>
    void operator()(MatchT& m) const
    {
        m.trees.clear();
    }
};

const node_parser_gen<discard_node_op> discard_node_d =
    node_parser_gen<discard_node_op>();

struct leaf_node_op
{
    template <typename MatchT>
    void operator()(MatchT& m) const
    {
        if (m.trees.size() == 1)
        {
            m.trees.begin()->children.clear();
        }
        else if (m.trees.size() > 1)
        {
            typedef typename MatchT::node_factory_t node_factory_t;
            m = MatchT(m.length(), node_factory_t::group_nodes(m.trees));
        }
    }
};

const node_parser_gen<leaf_node_op> leaf_node_d =
    node_parser_gen<leaf_node_op>();
const node_parser_gen<leaf_node_op> token_node_d =
    node_parser_gen<leaf_node_op>();

struct infix_node_op
{
    template <typename MatchT>
    void operator()(MatchT& m) const
    {
        typedef typename MatchT::container_t container_t;
        typedef typename MatchT::container_t::iterator iter_t;
        typedef typename MatchT::container_t::value_type value_t;

        // copying the tree nodes is expensive, since it may copy a whole
        // tree.  swapping them is cheap, so swap the nodes we want into
        // a new container of children.
        bool keep = true;
        container_t new_children;
        for (iter_t i = m.trees.begin(); i != m.trees.end(); ++i)
        {
            if (keep)
            {
                // move the child node
                new_children.push_back(value_t());
                impl::cp_swap(new_children.back(), *i);
                keep = false;
            }
            else
            {
                // ignore this child node
                keep = true;
            }
        }
        impl::cp_swap(m.trees, new_children);
    }
};

const node_parser_gen<infix_node_op> infix_node_d =
    node_parser_gen<infix_node_op>();

struct discard_first_node_op
{
    template <typename MatchT>
    void operator()(MatchT& m) const
    {
        typedef typename MatchT::container_t container_t;
        typedef typename MatchT::container_t::iterator iter_t;
        typedef typename MatchT::container_t::value_type value_t;

        // copying the tree nodes is expensive, since it may copy a whole
        // tree.  swapping them is cheap, so swap the nodes we want into
        // a new container of children, instead of saying
        // m.trees.erase(m.trees.begin()) because, on a vector that will cause
        // all the nodes afterwards to be copied into the previous position.
        container_t new_children;
        iter_t i = m.trees.begin();
        for (++i; i != m.trees.end(); ++i)
        {
            // move the child node
            new_children.push_back(value_t());
            impl::cp_swap(new_children.back(), *i);
        }
        impl::cp_swap(m.trees, new_children);
    }
};

const node_parser_gen<discard_first_node_op> discard_first_node_d =
    node_parser_gen<discard_first_node_op>();

struct discard_last_node_op
{
    template <typename MatchT>
    void operator()(MatchT& m) const
    {
        m.trees.pop_back();
    }
};

const node_parser_gen<discard_last_node_op> discard_last_node_d =
    node_parser_gen<discard_last_node_op>();

struct inner_node_op
{
    template <typename MatchT>
    void operator()(MatchT& m) const
    {
        typedef typename MatchT::container_t container_t;
        typedef typename MatchT::container_t::iterator iter_t;
        typedef typename MatchT::container_t::value_type value_t;

        // copying the tree nodes is expensive, since it may copy a whole
        // tree.  swapping them is cheap, so swap the nodes we want into
        // a new container of children, instead of saying
        // m.trees.erase(m.trees.begin()) because, on a vector that will cause
        // all the nodes afterwards to be copied into the previous position.
        container_t new_children;
        m.trees.pop_back(); // erase the last element
        iter_t i = m.trees.begin(); // skip over the first element
        for (++i; i != m.trees.end(); ++i)
        {
            // move the child node
            new_children.push_back(value_t());
            impl::cp_swap(new_children.back(), *i);
        }
        impl::cp_swap(m.trees, new_children);
    }
};

const node_parser_gen<inner_node_op> inner_node_d =
    node_parser_gen<inner_node_op>();


//////////////////////////////////
// action_directive_parser and action_directive_parser_gen
// are meant to be used as a template to create directives that
// generate action classes.  For example access_match and
// access_node.  The ActionParserT template parameter must be
// a class that has an innter class called action that is templated
// on the parser type and the action type.
template <typename ActionParserT>
struct action_directive_parser_gen;

//////////////////////////////////
// Calls the attached action passing it the match from the parser
// and the first and last iterators
struct access_match_action
{
    // inner template class to simulate template-template parameters
    template <typename ParserT, typename ActionT>
    struct action_def
    :   public unary<ParserT, parser<access_match_action::action_def<ParserT, ActionT> > >
    {
        typedef action_parser_category parser_category;
        typedef action_def<ParserT, ActionT> self_t;

        action_def( ParserT const& subject,
                ActionT const& actor_);

        template <typename ScannerT>
        struct result
        {
            typedef typename parser_result<ParserT,ScannerT>::type type;
        };

        template <typename ScannerT>
        typename parser_result<self_t, ScannerT>::type
        parse(ScannerT const& scan) const {
            typedef typename ScannerT::iterator_t iterator_t;
            typedef typename parser_result<self_t, ScannerT>::type result_t;
            if (!scan.at_end())
            {
                iterator_t save = scan.first;
                result_t hit = this->subject().parse(scan);
                actor(hit, save, scan.first);
                return hit;
            }
            return scan.no_match();
        }

        ActionT const &predicate() const;

        private:
        ActionT actor;
    };
};

//////////////////////////////////
template <typename ParserT, typename ActionT>
access_match_action::action_def<ParserT, ActionT>::action_def(
    ParserT const& subject,
    ActionT const& actor_)
: unary<ParserT, parser<access_match_action::action_def<ParserT, ActionT> > >(subject)
, actor(actor_)
{}

//////////////////////////////////
template <typename ParserT, typename ActionT>
ActionT const &access_match_action::action_def<ParserT, ActionT>::predicate() const
{
    return actor;
}

//////////////////////////////////
// Calls the attached action passing it the node from the parser
// and the first and last iterators
struct access_node_action
{
    // inner template class to simulate template-template parameters
    template <typename ParserT, typename ActionT>
    struct action_def
    :   public unary<ParserT, parser<access_node_action::action_def<ParserT, ActionT> > >
    {
        typedef action_parser_category parser_category;
        typedef action_def<ParserT, ActionT> self_t;
        typedef unary<ParserT, parser<access_node_action::action_def<ParserT, ActionT> > > base_t;

        action_def( ParserT const& subject,
                ActionT const& actor_);

        template <typename ScannerT>
        struct result
        {
            typedef typename parser_result<ParserT,ScannerT>::type type;
        };

        template <typename ScannerT>
        typename parser_result<self_t, ScannerT>::type
        parse(ScannerT const& scan) const
        {
            typedef typename ScannerT::iterator_t iterator_t;
            typedef typename parser_result<self_t, ScannerT>::type result_t;
            if (!scan.at_end())
            {
                iterator_t save = scan.first;
                result_t hit = this->subject().parse(scan);
                if (hit && hit.trees.size() > 0)
                    actor(*hit.trees.begin(), save, scan.first);
                return hit;
            }
            return scan.no_match();
        }

        ActionT const &predicate() const;

        private:
        ActionT actor;
    };
};

//////////////////////////////////
template <typename ParserT, typename ActionT>
access_node_action::action_def<ParserT, ActionT>::action_def(
    ParserT const& subject,
    ActionT const& actor_)
: base_t(subject)
, actor(actor_)
{}

//////////////////////////////////
template <typename ParserT, typename ActionT>
ActionT const &access_node_action::action_def<ParserT, ActionT>::predicate() const
{
    return actor;
}

template <typename T, typename ActionParserT>
struct action_directive_parser
:   public unary<T, parser<action_directive_parser<T, ActionParserT> > >
{
    typedef action_directive_parser<T, ActionParserT> self_t;
    typedef action_directive_parser_gen<ActionParserT> parser_generator_t;
    typedef unary_parser_category parser_category_t;
    typedef unary<T, parser<action_directive_parser<T, ActionParserT> > > base_t;
//    typedef action_directive_parser<T, ActionParserT> const &embed_t;

    action_directive_parser(T const& a)
        : base_t(a) {}

    template <typename ScannerT>
    typename parser_result<self_t, ScannerT>::type
    parse(ScannerT const& scanner) const
    {
        return this->subject().parse(scanner);
    }

    template <typename ScannerT>
    struct result
    {
        typedef typename parser_result<T,ScannerT>::type type;
    };

    template <typename ParserT,typename ActionT>
    struct subscript_result {
#if! BOOST_WORKAROUND( BOOST_MSVC, <= 1300)
        typedef typename directive_parser<ActionParserT,ParserT, ActionT>::type type;
#else //This is not a very beautiful hack, any other suggestions would be greatly appreciated.
        typedef mpl::if_<
            is_same<ActionParserT,access_node_action>,
            access_node_action::action_def<ParserT,ActionT>,
            access_match_action::action_def<ParserT,ActionT>
        >::type type;
#endif
    };

    template <typename ActionT>
    typename subscript_result<self_t,ActionT>::type 
    operator[](ActionT const& actor) const
    {
        typedef typename subscript_result<self_t,ActionT>::type 
            action_t;
        return action_t(*this, actor);
    }
};

//////////////////////////////////
template <typename ActionParserT>
struct action_directive_parser_gen
{
    template <typename T>
    struct result {

        typedef action_directive_parser<T, ActionParserT> type;
    };

    template <typename T>
    static action_directive_parser<T, ActionParserT>
    generate(parser<T> const& s)
    {
        return action_directive_parser<T, ActionParserT>(s.derived());
    }

    template <typename T>
    action_directive_parser<T, ActionParserT>
    operator[](parser<T> const& s) const
    {
        return action_directive_parser<T, ActionParserT>(s.derived());
    }
};

//////////////////////////////////
const action_directive_parser_gen<access_match_action> access_match_d
    = action_directive_parser_gen<access_match_action>();

//////////////////////////////////
const action_directive_parser_gen<access_node_action> access_node_d
    = action_directive_parser_gen<access_node_action>();


//////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
//  tree_parse_info
//
//      Results returned by the tree parse functions:
//
//      stop:   points to the final parse position (i.e parsing
//              processed the input up to this point).
//
//      match:  true if parsing is successful. This may be full:
//              the parser consumed all the input, or partial:
//              the parser consumed only a portion of the input.
//
//      full:   true when we have a full match (i.e the parser
//              consumed all the input.
//
//      length: The number of characters consumed by the parser.
//              This is valid only if we have a successful match
//              (either partial or full). A negative value means
//              that the match is unsucessful.
//
//     trees:   Contains the root node(s) of the tree.
//
///////////////////////////////////////////////////////////////////////////////
template <
    typename IteratorT = char const *,
    typename NodeFactoryT = node_val_data_factory<nil_t>,
    typename T = nil_t
>
struct tree_parse_info {

    IteratorT   stop;
    bool        match;
    bool        full;
    std::size_t length;
    typename tree_match<IteratorT, NodeFactoryT, T>::container_t trees;

    tree_parse_info()
        : stop()
        , match(false)
        , full(false)
        , length(0)
        , trees()
    {}

#if! BOOST_WORKAROUND( BOOST_MSVC, <= 1300)
    template <typename IteratorT2>
    tree_parse_info(tree_parse_info<IteratorT2> & pi)
        : stop(pi.stop)
        , match(pi.match)
        , full(pi.full)
        , length(pi.length)
        , trees()
    {
        // use auto_ptr like ownership for the trees data member
        impl::cp_swap(trees, pi.trees);
    }
#endif

/*    tree_parse_info(tree_parse_info & pi)
        : stop(pi.stop)
        , match(pi.match)
        , full(pi.full)
        , length(pi.length)
        , trees()
    {
        // use auto_ptr like ownership for the trees data member
        impl::cp_swap(trees, pi.trees);
    }*/
    tree_parse_info(tree_parse_info const & pi)
        : stop(pi.stop)
        , match(pi.match)
        , full(pi.full)
        , length(pi.length)
        , trees()
    {
        typedef typename tree_match<IteratorT, NodeFactoryT, T>::container_t trees_type;
        // use auto_ptr like ownership for the trees data member
        impl::cp_swap(trees, const_cast<trees_type&>(pi.trees));
    }


    tree_parse_info(
        IteratorT   stop_,
        bool        match_,
        bool        full_,
        std::size_t length_,
        typename tree_match<IteratorT, NodeFactoryT, T>::container_t trees_)
    :   stop(stop_)
        , match(match_)
        , full(full_)
        , length(length_)
        , trees()
    {
        // use auto_ptr like ownership for the trees data member
        impl::cp_swap(trees, trees_);
    }
};

}} // namespace boost::spirit

#undef BOOST_SPIRIT_IT_NS
#undef BOOST_SPIRIT_MP_TYPE_COMPUTER_ARGS

#endif

