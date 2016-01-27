// Copyright Peder Holt 2003-2004. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#if !defined(BOOST_SPIRIT_LITERAL_DIRECTIVE_HPP)
#define BOOST_SPIRIT_LITERAL_DIRECTIVE_HPP

#include <boost/spirit/include/classic_parser.hpp>

namespace boost { namespace spirit { namespace classic {

    namespace impl
    {
       inline void breaker2()
       {
          int a=0;
       }
        template <typename RT, typename ST, typename ScannerT, typename BaseT>
        inline RT
        literal_parser_parse(
            ST const& s,
            ScannerT const& scan,
            skipper_iteration_policy<BaseT> const&,RT* dummy=0)
        {
            typedef scanner_policies<
                no_skipper_iteration_policy<
                    BOOST_DEDUCED_TYPENAME ScannerT::iteration_policy_t>,
                BOOST_DEDUCED_TYPENAME ScannerT::match_policy_t,
                BOOST_DEDUCED_TYPENAME ScannerT::action_policy_t
            > policies_t;
            breaker2();
            typedef typename ScannerT::iterator_t iterator_t;
            iterator_t save=scan.first;
            if(!scan.at_end()) {
               iterator_t symbol_start=scan.first;
               bool alpha_start = eps_p(alpha_p | '_' | '$').parse(scan.change_policies(policies_t(scan)));
               RT hit = s.parse(scan.change_policies(policies_t(scan)));
               //Is this match a literal?
               if(alpha_start && hit) {
                   //Is this literal closed?
                   bool alpha_end= eps_p(alnum_p | '_' | '$').parse(scan.change_policies(policies_t(scan)));
                   //No, then enforce a skip at end of match.                  
                   if(alpha_end) {
                       scan.first = save;
                       return scan.no_match();
                   }
                   return hit;
               }
               //If this match is not a literal, don't do a check on the post-string.
               //Prevent ++ to be parsed as + +
               else if(hit && *symbol_start!=*scan.first) return hit;
            }
            scan.first=save;
            return scan.no_match();
        }

        template <typename RT, typename ST, typename ScannerT, typename BaseT>
        inline RT
        literal_parser_parse(
            ST const& s,
            ScannerT const& scan,
            no_skipper_iteration_policy<BaseT> const&,RT* dummy=0)
        {
            return s.parse(scan);
        }

        template <typename RT, typename ST, typename ScannerT>
        inline RT
        literal_parser_parse(
            ST const& s,
            ScannerT const& scan,
            iteration_policy const&,RT* dummy=0)
        {
            return s.parse(scan);
        }
    }

    struct literal_parser_gen;

    template <typename ParserT>
    struct literal_parser
    :   public unary<ParserT, parser<literal_parser<ParserT> > >
    {
        typedef literal_parser<ParserT>         self_t;
        typedef unary_parser_category           parser_category_t;
        typedef literal_parser_gen              parser_generator_t;
        typedef unary<ParserT, parser<self_t> > base_t;

        template <typename ScannerT>
        struct result
        {
            typedef typename parser_result<ParserT, ScannerT>::type type;
        };

        literal_parser(ParserT const& p)
        : base_t(p) {}

        template <typename ScannerT>
        typename parser_result<self_t, ScannerT>::type
        parse(ScannerT const& scan) const
        {
            typedef typename parser_result<self_t, ScannerT>::type result_t;
            return impl::literal_parser_parse<result_t>
                (this->subject(), scan, scan);
        }
    };
    
    struct literal_parser_gen
    {
        template <typename ParserT>
        struct result {

            typedef literal_parser<ParserT> type;
        };

        template <typename ParserT>
        static literal_parser<ParserT>
        generate(parser<ParserT> const& subject)
        {
            return literal_parser<ParserT>(subject.derived());
        }

        template <typename ParserT>
        literal_parser<ParserT>
        operator[](parser<ParserT> const& subject) const
        {
            return literal_parser<ParserT>(subject.derived());
        }
    };

    static literal_parser_gen literal_d=literal_parser_gen();

}}}//namespace boost::spirit::classic

#endif

