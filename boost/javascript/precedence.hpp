// Copyright Peder Holt 2003-2004. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#if !defined(BOOST_SPIRIT_PRECEDENCE_HPP)
#define BOOST_SPIRIT_PRECEDENCE_HPP

#include <boost/spirit/include/classic_parser.hpp>
#include <boost/spirit/include/classic_primitives.hpp>
#include <boost/spirit/include/classic_composite.hpp>

namespace boost { namespace spirit { namespace classic {

    #if defined(BOOST_MSVC) && (BOOST_MSVC <= 1300)

    BOOST_SPIRIT_DEPENDENT_TEMPLATE_WRAPPER(operator_generator_wrapper, parser_gen);

    //////////////////////////////////
    template <typename GeneratorA, typename GeneratorB>
    struct operator_generator
    {
        typedef typename impl::operator_generator_wrapper<GeneratorA>
            ::template result_<GeneratorB>::param_t::type type;
    };

    #else

    //////////////////////////////////
    template <typename GeneratorA, typename GeneratorB>
    struct operator_generator
    {
        typedef typename GeneratorA::template parser_gen<GeneratorB>::type type;
    };

    #endif

    template<typename ExpressionP>
    struct no_operator_parser
    : public unary<ExpressionP, parser<no_operator_parser<ExpressionP> > >
    {
        typedef no_operator_parser<ExpressionP>         self_t;
        typedef unary_parser_category                   parser_category_t;
        typedef unary<ExpressionP, parser<self_t> >     base_t;

        no_operator_parser(ExpressionP const& subject)
        : base_t(subject) {}

        template <typename ScannerT>
        typename parser_result<self_t, ScannerT>::type
        parse(ScannerT const& scan,bool& altered) const {
            return subject().parse(scan);
        };
    };

    struct no_operator_parser_gen {
        typedef no_operator_parser_gen self_t;
        typedef no_operator_parser_gen const& embed_t;

        template<typename ExpressionP>
        struct result
        {
            typedef no_operator_parser<ExpressionP> type;
        };

        template<typename NextParserG>
        struct parser_gen
        {
            typedef no_operator_parser_gen type;
        };

        template<typename ExpressionP>
        static typename parser_result<self_t,ExpressionP>::type
        generate(self_t const& self,ExpressionP const& expr)
        {
            typedef typename parser_result<self_t,ExpressionP>::type result_t;
            return result_t(expr);
        }                
    };

    template<typename DerivedT>
    struct operator_parser_gen 
    {        
        DerivedT const& derived() const {return static_cast<DerivedT const&>(*this);}
   
        template<typename ParserG>
        typename operator_generator<ParserG,DerivedT>::type
        operator,(operator_parser_gen<ParserG> const& gen) const {
            typedef typename operator_generator<ParserG,DerivedT>::type next_t;
            return next_t(gen.derived(),derived());
        }
    };


    template<typename OperatorP,typename NextParserG = no_operator_parser_gen>
    struct postfix_operator_parser_gen;

    template<typename NextP,typename OperatorP>
    struct postfix_operator_parser 
    : public binary<NextP, OperatorP, parser<postfix_operator_parser<NextP,OperatorP> > >
    {
        typedef postfix_operator_parser<NextP,OperatorP>    self_t;
        typedef binary_parser_category                      parser_category_t;
        typedef binary<NextP, OperatorP, parser<self_t> >   base_t;
        typedef typename base_t::left_t                      left_t;
        typedef typename base_t::right_t                     right_t;

        postfix_operator_parser(NextP const& next,OperatorP const& op) 
        :   base_t(next,op)
        {}

        template <typename ScannerT>
        typename parser_result<self_t, ScannerT>::type
        parse(ScannerT const& scan) const {
            bool altered=false;
            return parse(scan,altered);
        };

        template <typename ScannerT>
        typename parser_result<self_t, ScannerT>::type
        parse(ScannerT const& scan,bool& altered) const {
            typedef typename ScannerT::iterator_t iterator_t;
            typedef typename parser_result<self_t, ScannerT>::type result_t;
            typedef typename parser_result<right_t, ScannerT>::type operator_t;

            iterator_t save(scan.first);
            if(result_t expr=left().parse(scan,altered)) {
                if(operator_t op=right().parse(scan)) {
                    scan.concat_match(expr,op);
                    scan.group_match(expr, op.value().get(), save, scan.first);
                    altered=true;
                }
                return expr;
            }
            else return scan.no_match();
        };
    };

    template<typename OperatorP,typename NextParserG>
    struct postfix_operator_parser_gen 
    : binary<OperatorP,NextParserG,operator_parser_gen<postfix_operator_parser_gen<OperatorP,NextParserG> > >
    {
        typedef postfix_operator_parser_gen<OperatorP,NextParserG>          self_t;
        typedef binary<OperatorP,NextParserG,operator_parser_gen<self_t> >  base_t;
        typedef self_t                                                      embed_t;
        typedef typename base_t::left_t                      left_t;
        typedef typename base_t::right_t                     right_t;

        postfix_operator_parser_gen(OperatorP const& left,NextParserG const& right)
        :   base_t(left,right)
        {}

        template<typename A,typename B>
        postfix_operator_parser_gen(postfix_operator_parser_gen<A,B> const& self,NextParserG const& right)
        :   base_t(self.left(),right)
        {}

        template<typename ExpressionP>
        struct result
        {
            typedef 
                postfix_operator_parser<
                    typename parser_result<NextParserG,ExpressionP>::type,
                    OperatorP
                > type;
        };
        
        template<typename ExpressionP>
        static typename parser_result<self_t,ExpressionP>::type
        generate(self_t const& self,ExpressionP const& expr)
        {
            typedef typename parser_result<self_t,ExpressionP>::type result_t;
            return result_t(NextParserG::generate(self.right(),expr),self.left());
        }

        template<typename NextParserG2>
        struct parser_gen
        {
            typedef postfix_operator_parser_gen<OperatorP,NextParserG2> type;
        };
    };

    template<typename OperatorP>
    postfix_operator_parser_gen<OperatorP>
    postfix_operator_p(OperatorP const& op) {
        return postfix_operator_parser_gen<OperatorP>(op,no_operator_parser_gen());
    }

    template<typename OperatorP,typename NextParserG = no_operator_parser_gen>
    struct unary_operator_parser_gen;

    template<typename OperatorP,typename NextP>
    struct unary_operator_parser 
    : public binary<OperatorP, NextP, parser<unary_operator_parser<OperatorP,NextP> > >
    {
        typedef unary_operator_parser<OperatorP,NextP>      self_t;
        typedef binary_parser_category                      parser_category_t;
        typedef binary<OperatorP,NextP, parser<self_t> >     base_t;
        typedef typename base_t::left_t                      left_t;
        typedef typename base_t::right_t                     right_t;


        unary_operator_parser(OperatorP const& op,NextP const& next) 
        :   base_t(op,next)
        {}

        template <typename ScannerT>
        typename parser_result<self_t, ScannerT>::type
        parse(ScannerT const& scan) const {
            bool altered=false;
            return parse(scan,altered);
        };

        template <typename ScannerT>
        typename parser_result<self_t, ScannerT>::type
        parse(ScannerT const& scan,bool& altered) const {
            typedef typename ScannerT::iterator_t iterator_t;
            typedef typename parser_result<self_t, ScannerT>::type result_t;
            typedef typename parser_result<left_t, ScannerT>::type operator_t;

            iterator_t save(scan.first);
            if(operator_t op=left().parse(scan)) {
                if(result_t expr=parse(scan,altered)) {
                    scan.concat_match(op, expr);
                    scan.group_match(op, op.value().get(), save, scan.first);
                    altered=true;
                    return op;
                }
                else return scan.no_match();
            }
            else return right().parse(scan,altered);
        };
    };


    template<typename OperatorP,typename NextParserG>
    struct unary_operator_parser_gen 
    : binary<OperatorP,NextParserG,operator_parser_gen<unary_operator_parser_gen<OperatorP,NextParserG> > >
    {
        typedef unary_operator_parser_gen<OperatorP,NextParserG>            self_t;
        typedef binary<OperatorP,NextParserG,operator_parser_gen<self_t> >  base_t;
        typedef self_t                                                      embed_t;
        typedef typename base_t::left_t                      left_t;
        typedef typename base_t::right_t                     right_t;

        unary_operator_parser_gen(OperatorP const& left,NextParserG const& right)
        :   base_t(left,right)
        {}

        template<typename A,typename B>
        unary_operator_parser_gen(unary_operator_parser_gen<A,B> const& self,NextParserG const& right)
        :   base_t(self.left(),right)
        {}

        template<typename ExpressionP>
        struct result
        {
            typedef 
                unary_operator_parser<
                    OperatorP,
                    typename parser_result<NextParserG,ExpressionP>::type                    
                > type;
        };
        
        template<typename ExpressionP>
        static typename parser_result<self_t,ExpressionP>::type
        generate(self_t const& self,ExpressionP const& expr)
        {
            typedef typename parser_result<self_t,ExpressionP>::type result_t;
            return result_t(self.left(),NextParserG::generate(self.right(),expr));
        }

        template<typename NextParserG2>
        struct parser_gen
        {
            typedef unary_operator_parser_gen<OperatorP,NextParserG2> type;
        };
    };

    template<typename OperatorP>
    unary_operator_parser_gen<OperatorP>
    unary_operator_p(OperatorP const& op) {
        return unary_operator_parser_gen<OperatorP>(op,no_operator_parser_gen());
    }

    template<typename OperatorP,typename NextParserG = no_operator_parser_gen>
    struct binary_operator_parser_gen;

    template<typename NextP,typename OperatorP>
    struct binary_operator_parser 
    : public binary<NextP, OperatorP, parser<binary_operator_parser<NextP,OperatorP> > >
    {
        typedef binary_operator_parser<NextP,OperatorP>      self_t;
        typedef binary_parser_category                       parser_category_t;
        typedef binary<NextP,OperatorP, parser<self_t> >     base_t;
        typedef typename base_t::left_t                      left_t;
        typedef typename base_t::right_t                     right_t;


        binary_operator_parser(NextP const& next,OperatorP const& op) 
        :   base_t(next,op)
        {}

        template <typename ScannerT>
        typename parser_result<self_t, ScannerT>::type
        parse(ScannerT const& scan) const {
            bool altered=false;
            return parse(scan,altered);
        };

        template <typename ScannerT>
        typename parser_result<self_t, ScannerT>::type
        parse(ScannerT const& scan,bool& altered) const {
            typedef typename parser_result<self_t, ScannerT>::type result_t;
            typedef typename parser_result<right_t, ScannerT>::type operator_t;
            typedef typename ScannerT::iterator_t iterator_t;

            if(result_t lhs = this->left().parse(scan,altered)) {
                iterator_t save(scan.first);
                if(operator_t op = this->right().parse(scan)) {
                    if(result_t rhs = this->left().parse(scan)) {
                        altered=true;
                        return parse_precedence(lhs,op,rhs,scan);
                    }
                    else {
                        scan.first=save;
                        return lhs;
                    }
                }
                else return lhs;
            }
            return scan.no_match();
        };

        template <typename ScannerT>
        typename parser_result<self_t, ScannerT>::type
        parse_precedence(
            typename parser_result<self_t, ScannerT>::type& lhs,
            typename parser_result<right_t, ScannerT>::type& op,
            typename parser_result<self_t, ScannerT>::type& rhs,
            ScannerT const& scan,long max_precedence=-1) const
        {
            typedef typename ScannerT::iterator_t iterator_t;
            typedef typename parser_result<self_t, ScannerT>::type result_t;
            typedef typename parser_result<right_t, ScannerT>::type operator_t;
    //                scan.first = save;  // fails.

            iterator_t save(scan.first);
            if(operator_t next_op = this->right().parse(scan)) {
                //Parse right tree first
                if(op.value() > next_op.value()) {
                    if(result_t next_rhs = this->left().parse(scan)) {
                        if(rhs = parse_precedence(rhs,next_op,next_rhs,scan,op.value())) {
                            return parse_precedence(lhs,op,rhs,scan,max_precedence);
                        }
                        else {
                            scan.concat_match(next_op, next_rhs);
                            scan.concat_match(rhs, next_op);
                            scan.group_match(rhs, next_op.value().get(), scan.first, scan.first);
                            scan.concat_match(op, rhs);
                            scan.concat_match(lhs, op);
                            scan.group_match(lhs, op.value().get(), scan.first, scan.first);
                            return lhs;
                        }
                    }
                    else return scan.no_match();
                }
                //Combine left tree
                else if(max_precedence>=0 && next_op.value()>=max_precedence) {
                    scan.first=save;
                    scan.concat_match(op, rhs);
                    scan.concat_match(lhs, op);
                    scan.group_match(lhs, op.value().get(), scan.first, scan.first);
                    return lhs;
                }
                //Combine left tree and parse on
                else {
                    scan.concat_match(op, rhs);
                    scan.concat_match(lhs, op);
                    scan.group_match(lhs, op.value().get(), scan.first, scan.first);
                    if(result_t next_rhs = this->left().parse(scan)) {
                        return parse_precedence(lhs,next_op,next_rhs,scan,max_precedence);
                    }
                    else return scan.no_match();
                }
            }
            else {
                scan.concat_match(op, rhs);
                scan.concat_match(lhs, op);
                scan.group_match(lhs, op.value().get(), scan.first, scan.first);
                return lhs;
            }
        }
    };

    template<typename OperatorP,typename NextParserG>
    struct binary_operator_parser_gen 
    : binary<OperatorP,NextParserG,operator_parser_gen<binary_operator_parser_gen<OperatorP,NextParserG> > >
    {
        typedef binary_operator_parser_gen<OperatorP,NextParserG>           self_t;
        typedef binary<OperatorP,NextParserG,operator_parser_gen<self_t> >  base_t;
        typedef self_t                                                      embed_t;
        typedef typename base_t::left_t                      left_t;
        typedef typename base_t::right_t                     right_t;

        binary_operator_parser_gen(OperatorP const& left,NextParserG const& right)
        :   base_t(left,right)
        {}

        template<typename A,typename B>
        binary_operator_parser_gen(binary_operator_parser_gen<A,B> const& self,NextParserG const& right)
        :   base_t(self.left(),right)
        {}

        template<typename ExpressionP>
        struct result
        {
            typedef 
                binary_operator_parser<
                    typename parser_result<NextParserG,ExpressionP>::type,
                    OperatorP
                > type;
        };
        
        template<typename ExpressionP>
        static typename parser_result<self_t,ExpressionP>::type
        generate(self_t const& self,ExpressionP const& expr)
        {
            typedef typename parser_result<self_t,ExpressionP>::type result_t;
            return result_t(NextParserG::generate(self.right(),expr),self.left());
        }

        template<typename NextParserG2>
        struct parser_gen
        {
            typedef binary_operator_parser_gen<OperatorP,NextParserG2> type;
        };
    };

    template<typename OperatorP>
    binary_operator_parser_gen<OperatorP>
    binary_operator_p(OperatorP const& op) {
        return binary_operator_parser_gen<OperatorP>(op,no_operator_parser_gen());
    }

    template<typename OperatorP,typename RestP,typename NextParserG = no_operator_parser_gen>
    struct ternary_operator_parser_gen;

    template<typename NextP,typename OperatorP,typename RestP>
    struct ternary_operator_parser 
    : public binary<NextP, OperatorP, parser<ternary_operator_parser<NextP,OperatorP,RestP> > >
    {
        typedef ternary_operator_parser<NextP,OperatorP,RestP>  self_t;
        typedef binary_parser_category                          parser_category_t;
        typedef binary<NextP,OperatorP, parser<self_t> >        base_t;
        typedef typename base_t::left_t                      left_t;
        typedef typename base_t::right_t                     right_t;


        ternary_operator_parser(NextP const& next,OperatorP const& op,RestP const& expr) 
        :   base_t(next,op)
        ,   rest(expr)
        {}

        template <typename ScannerT>
        typename parser_result<self_t, ScannerT>::type
        parse(ScannerT const& scan) const {
            bool altered=false;
            return parse(scan,altered);
        };

        template <typename ScannerT>
        typename parser_result<self_t, ScannerT>::type
        parse(ScannerT const& scan,bool& altered) const {
            typedef typename parser_result<self_t, ScannerT>::type result_t;
            typedef typename parser_result<right_t, ScannerT>::type operator_t;

            if(result_t lhs = this->left().parse(scan,altered)) {
                operator_t op = this->right().parse(scan);
                if(op) {
                    altered=true;
                    if(result_t rhs = rest.parse(scan)) {
                        scan.concat_match(op, rhs);
                        scan.concat_match(lhs, op);
                        scan.group_match(lhs, op.value().get(), scan.first, scan.first);
                    }
                }
                return lhs;
            }
            return scan.no_match();
        }
        RestP rest;
    };

    template<typename OperatorP,typename RestP,typename NextParserG>
    struct ternary_operator_parser_gen 
    : binary<OperatorP,NextParserG,operator_parser_gen<ternary_operator_parser_gen<OperatorP,RestP,NextParserG> > >
    {
        typedef ternary_operator_parser_gen<OperatorP,RestP,NextParserG>        self_t;
        typedef binary<OperatorP,NextParserG,operator_parser_gen<self_t> >      base_t;
        typedef self_t                                                          embed_t;
        typedef typename base_t::left_t                      left_t;
        typedef typename base_t::right_t                     right_t;

        ternary_operator_parser_gen(OperatorP const& left,RestP const& expr,NextParserG const& right)
        :   base_t(left,right)
        ,   rest(expr)
        {}

        template<typename A,typename B,typename C>
        ternary_operator_parser_gen(ternary_operator_parser_gen<A,B,C> const& self,NextParserG const& right)
        :   base_t(self.left(),right)
        ,   rest(self.rest)
        {}

        template<typename ExpressionP>
        struct result
        {
            typedef 
                ternary_operator_parser<
                    typename parser_result<NextParserG,ExpressionP>::type,
                    OperatorP,
                    RestP
                > type;
        };
        
        template<typename ExpressionP>
        static typename parser_result<self_t,ExpressionP>::type
        generate(self_t const& self,ExpressionP const& expr)
        {
            typedef typename parser_result<self_t,ExpressionP>::type result_t;
            return result_t(NextParserG::generate(self.right(),expr),self.left(),self.rest);
        }

        template<typename NextParserG2>
        struct parser_gen
        {
            typedef ternary_operator_parser_gen<OperatorP,RestP,NextParserG2> type;
        };
        RestP rest;
    };

    template<typename OperatorP,typename RestP>
    ternary_operator_parser_gen<OperatorP,RestP>
    ternary_operator_p(OperatorP const& op,RestP const& rest) {
        return ternary_operator_parser_gen<OperatorP,RestP>(op,rest,no_operator_parser_gen());
    }

    template<typename OperatorP,typename NextParserG = no_operator_parser_gen>
    struct binary_assignment_operator_parser_gen;

    template<typename NextP,typename OperatorP>
    struct binary_assignment_operator_parser 
    : public binary<NextP, OperatorP, parser<binary_assignment_operator_parser<NextP,OperatorP> > >
    {
        typedef binary_assignment_operator_parser<NextP,OperatorP>  self_t;
        typedef binary_parser_category                          parser_category_t;
        typedef binary<NextP,OperatorP, parser<self_t> >        base_t;
        typedef typename base_t::left_t                      left_t;
        typedef typename base_t::right_t                     right_t;


        binary_assignment_operator_parser(NextP const& next,OperatorP const& op) 
        :   base_t(next,op)
        {}

        template <typename ScannerT>
        typename parser_result<self_t, ScannerT>::type
        parse(ScannerT const& scan) const {
            bool altered=false;
            return parse(scan,altered);
        };

        template <typename ScannerT>
        typename parser_result<self_t, ScannerT>::type
        parse(ScannerT const& scan,bool& altered) const {
            typedef typename parser_result<self_t, ScannerT>::type result_t;
            typedef typename parser_result<right_t, ScannerT>::type operator_t;
            
            altered = false;
            if(result_t lhs = this->left().parse(scan,altered)) {
                if(altered) return lhs;
                if(operator_t op = this->right().parse(scan)) {
                    if(result_t rhs = this->left().parse(scan,altered)) {
                        scan.concat_match(op, rhs);
                        scan.concat_match(lhs, op);
                        scan.group_match(lhs, op.value().get(), scan.first, scan.first);
                        return lhs;
                    }
                    return lhs;
                }
                else return lhs;
            }

            return scan.no_match();
        }
    };

    template<typename OperatorP,typename NextParserG>
    struct binary_assignment_operator_parser_gen 
    : binary<OperatorP,NextParserG,operator_parser_gen<binary_assignment_operator_parser_gen<OperatorP,NextParserG> > >
    {
        typedef binary_assignment_operator_parser_gen<OperatorP,NextParserG>        self_t;
        typedef binary<OperatorP,NextParserG,operator_parser_gen<self_t> >      base_t;
        typedef self_t                                                          embed_t;
        typedef typename base_t::left_t                      left_t;
        typedef typename base_t::right_t                     right_t;

        binary_assignment_operator_parser_gen(OperatorP const& left,NextParserG const& right)
        :   base_t(left,right)
        {}

        template<typename A,typename B>
        binary_assignment_operator_parser_gen(binary_assignment_operator_parser_gen<A,B> const& self,NextParserG const& right)
        :   base_t(self.left(),right)
        {}

        template<typename ExpressionP>
        struct result
        {
            typedef 
                binary_assignment_operator_parser<
                    typename parser_result<NextParserG,ExpressionP>::type,
                    OperatorP
                > type;
        };
        
        template<typename ExpressionP>
        static typename parser_result<self_t,ExpressionP>::type
        generate(self_t const& self,ExpressionP const& expr)
        {
            typedef typename parser_result<self_t,ExpressionP>::type result_t;
            return result_t(NextParserG::generate(self.right(),expr),self.left());
        }

        template<typename NextParserG2>
        struct parser_gen
        {
            typedef binary_assignment_operator_parser_gen<OperatorP,NextParserG2> type;
        };
    };

    template<typename OperatorP>
    binary_assignment_operator_parser_gen<OperatorP>
    binary_assignment_operator_p(OperatorP const& op) {
        return binary_assignment_operator_parser_gen<OperatorP>(op,no_operator_parser_gen());
    }

    template<typename ExpressionP>
    struct precedence_parser 
    : unary<ExpressionP,parser<precedence_parser<ExpressionP> > >
    {
        typedef precedence_parser<ExpressionP>      self_t;
        typedef unary<ExpressionP,parser<self_t> >  base_t;
        
        precedence_parser(ExpressionP const& expr) : base_t(expr) {}
        
        template<typename DerivedT>
        typename parser_result<DerivedT,ExpressionP>::type
        operator[](operator_parser_gen<DerivedT> const& next)
        {
            typedef typename parser_result<DerivedT,ExpressionP>::type result_t;
            return result_t(DerivedT::generate(next.derived(),subject()));
        }
    };

    template<typename ExpressionP>
    precedence_parser<ExpressionP> precedence_p(ExpressionP const& expr) {
        return precedence_parser<ExpressionP>(expr);
    }

}}} // namespace boost::spirit::classic

#endif


