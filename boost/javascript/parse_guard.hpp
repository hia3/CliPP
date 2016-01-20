// Copyright Peder Holt 2003-2004. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#if !defined(BOOST_JAVASCRIPT_PARSE_GUARD_HPP)
#define BOOST_JAVASCRIPT_PARSE_GUARD_HPP

#include <boost/spirit/core/parser.hpp>
#include <boost/javascript/javascript_parser.hpp>
#include <boost/javascript/literal_directive.hpp>

namespace boost { namespace spirit {


    template <typename CharT>
    inline std::string
    parser_error_text(chlit<CharT> const &p)
    {
        return std::string("\'")
            + std::string(1, p.ch)
            + std::string("\'");
    }
    template <typename T>
    inline std::string
    parser_error_text(no_tree_gen_node_parser<T> const &p)
    {
        return parser_error_text(p.subject());
    }
    template <typename ScannerT,typename ContextT,typename TagT>
    inline std::string
    parser_error_text(rule<ScannerT,ContextT,TagT> const &p)
    {
        switch(p.id().to_long()) {
        case identifierID:
            return "'identifier'";
        case for_condition_standardID:
            return "for condition";
        case for_condition_inID:
            return "for condition in";
        default:
            if(p.id().to_long() > finally_clauseID)
                return "'expression'";
            else 
                return lexical_cast<std::string>(p.id().to_long());
        }
    }
    template <typename A, typename B>
    inline std::string
    parser_error_text(alternative<A,B> const &p)
    {
        return parser_error_text(p.left()) + " or " + parser_error_text(p.right());
    }
    template<typename ParserT>
    inline std::string
    parser_error_text(literal_parser<ParserT> const& p) {
        return parser_error_text(p.subject());
    }
    template<typename IteratorT>
    inline std::string
    parser_error_text(strlit<IteratorT> const& p) {
        return std::string(p.seq().first(),p.seq().last());
    }

    struct parse_guard_gen;

    template<typename ParserT>
    struct parse_guard 
    : unary<ParserT,parser<parse_guard<ParserT> > >
    {
        typedef parse_guard<ParserT>            self_t;
        typedef unary_parser_category           parser_category_t;
        typedef parse_guard_gen                 parser_generator_t;
        typedef unary<ParserT,parser<self_t> >  base_t;
        
        template <typename ScannerT>
        struct result
        {
            typedef typename parser_result<ParserT, ScannerT>::type type;
        };

        parse_guard() :base_t() {}
        parse_guard(ParserT const& expr) 
        : base_t(expr) {}

        template <typename ScannerT>
        typename parser_result<ParserT, ScannerT>::type
        parse(ScannerT const& scan) const
        {
            typedef typename parser_result<ParserT, ScannerT>::type result_t;
            typedef typename ScannerT::iterator_t iterator_t;
            iterator_t save(scan.first);
            if(result_t expr = subject().parse(scan)) return expr;
            else {
                scan.first=save;                
                throw javascript::parse_guard_triggered("Expected " + parser_error_text(subject()),scan.first);
            }
        }
    };

    template<typename ParserT>
    struct parse_guard_with_name 
    : unary<ParserT,parser<parse_guard_with_name<ParserT> > >
    {
        typedef parse_guard_with_name<ParserT>  self_t;
        typedef unary_parser_category           parser_category_t;
        typedef parse_guard_gen                 parser_generator_t;
        typedef unary<ParserT,parser<self_t> >  base_t;
        
        template <typename ScannerT>
        struct result
        {
            typedef typename parser_result<ParserT, ScannerT>::type type;
        };

        parse_guard_with_name() :base_t() {}
        parse_guard_with_name(ParserT const& expr,const char* name) 
            : base_t(expr) ,name_(name) {}

        template <typename ScannerT>
        typename parser_result<ParserT, ScannerT>::type
        parse(ScannerT const& scan) const
        {
            typedef typename parser_result<ParserT, ScannerT>::type result_t;
            typedef typename ScannerT::iterator_t iterator_t;
            iterator_t save(scan.first);
            if(result_t expr = subject().parse(scan)) return expr;
            else {
                scan.first=save;                
                throw javascript::parse_guard_triggered("Expected \'" + name_ + "\'",scan.first);
            }
        }
        std::string name_;
    };

    struct parse_guard_gen
    {
        template <typename ParserT>
        struct result {

            typedef parse_guard<ParserT> type;
        };

        template <typename ParserT>
        static parse_guard<ParserT>
        generate(parser<ParserT> const& subject)
        {
            return parse_guard<ParserT>(subject.derived());
        }
        template <typename ParserT>
        static parse_guard_with_name<ParserT>
        generate(parser<ParserT> const& subject,const char* name)
        {
            return parse_guard_with_name<ParserT>(subject.derived(),name);
        }

        template <typename ParserT>
        parse_guard<ParserT>
        operator[](parser<ParserT> const& subject) const
        {
            return parse_guard<ParserT>(subject.derived());
        }
        template <typename ParserT>
        parse_guard_with_name<ParserT>
        operator()(parser<ParserT> const& subject,const char* name) const
        {
            return parse_guard_with_name<ParserT>(subject.derived(),name);
        }
    };

    static parse_guard_gen parse_guard_d=parse_guard_gen();
}}

#endif