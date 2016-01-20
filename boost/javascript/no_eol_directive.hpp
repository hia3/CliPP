// Copyright Peder Holt 2003-2004. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#if !defined(BOOST_SPIRIT_NO_EOL_DIRECTIVE_HPP)
#define BOOST_SPIRIT_NO_EOL_DIRECTIVE_HPP

#include <boost/spirit/core/parser.hpp>

namespace boost { namespace spirit {

    struct skip_grammar_no_eol : public grammar<skip_grammar_no_eol>
    {
        template <typename ScannerT>
        struct definition
        {
            definition(skip_grammar_no_eol const& /*self*/)
            {
                skip=   (space_p - eol_p)
                    |   "//" >> *(anychar_p - '\n')      // C++ comment
                    |   "/*" >> *(anychar_p - (eol_p |"*/") ) >> "*/"     // C comment
                    ;
            }

            rule<ScannerT> skip;

            rule<ScannerT> const&
            start() const { return skip; }
        };
    };

    namespace impl
    {
        template <typename RT, typename ST, typename ScannerT, typename BaseT>
        inline RT
        no_eol_parser_parse(
            ST const& s,
            ScannerT const& scan,
            skipper_iteration_policy<BaseT> const&,RT* dummy=0)
        {
            typedef skip_parser_iteration_policy<skip_grammar_no_eol>  iter_policy_t;
            typedef scanner_policies<
                iter_policy_t,
                BOOST_DEDUCED_TYPENAME ScannerT::match_policy_t,
                BOOST_DEDUCED_TYPENAME ScannerT::action_policy_t
            > policies_t;
            
            skip_grammar_no_eol skip;            
            iter_policy_t iter_policy(skip);
            RT result = s.parse(scan.change_policies(policies_t(iter_policy)));
            return result;
        }

        template <typename RT, typename ST, typename ScannerT, typename BaseT>
        inline RT
        no_eol_parser_parse(
            ST const& s,
            ScannerT const& scan,
            no_skipper_iteration_policy<BaseT> const&,RT* dummy=0)
        {
            return s.parse(scan);
        }

        template <typename RT, typename ST, typename ScannerT>
        inline RT
        no_eol_parser_parse(
            ST const& s,
            ScannerT const& scan,
            iteration_policy const&,RT* dummy=0)
        {
            return s.parse(scan);
        }
    }

    struct no_eol_parser_gen;

    template <typename ParserT>
    struct no_eol_parser
    :   public unary<ParserT, parser<no_eol_parser<ParserT> > >
    {
        typedef no_eol_parser<ParserT>         self_t;
        typedef unary_parser_category           parser_category_t;
        typedef no_eol_parser_gen              parser_generator_t;
        typedef unary<ParserT, parser<self_t> > base_t;

        template <typename ScannerT>
        struct result
        {
            typedef typename parser_result<ParserT, ScannerT>::type type;
        };

        no_eol_parser() :base_t() {}
        no_eol_parser(ParserT const& p)
        : base_t(p) {}

        template <typename ScannerT>
        typename parser_result<ParserT, ScannerT>::type
        parse(ScannerT const& scan) const
        {
            typedef typename parser_result<ParserT, ScannerT>::type result_t;
            return impl::no_eol_parser_parse<result_t>
                (this->subject(), scan, scan);
        }
    };
    
    struct no_eol_parser_gen
    {
        template <typename ParserT>
        struct result {

            typedef no_eol_parser<ParserT> type;
        };

        template <typename ParserT>
        static no_eol_parser<ParserT>
        generate(parser<ParserT> const& subject)
        {
            return no_eol_parser<ParserT>(subject.derived());
        }

        template <typename ParserT>
        no_eol_parser<ParserT>
        operator[](parser<ParserT> const& subject) const
        {
            return no_eol_parser<ParserT>(subject.derived());
        }
    };

    static no_eol_parser_gen no_eol_d=no_eol_parser_gen();

}}//namespace boost::spirit

#endif

