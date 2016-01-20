#include "boost/spirit/core.hpp"
#include "boost/spirit/tree/ast.hpp"
#include <boost/javascript/javascript_parser.hpp>

#include <boost/clipp/context.hpp>
#include <boost/clipp/detail/operator_invoker.hpp>
#include <boost/javascript/expression_types.hpp>
#include <iostream>
#include <ctype.h>
#include <boost/next_prior.hpp>
#include <boost/spirit/tree/tree_to_xml.hpp>

#include <boost/clipp/class.hpp>

#include <boost/javascript/number.hpp>
#include <boost/javascript/math.hpp>
#include <boost/javascript/js_string.hpp>
#include <boost/javascript/boolean.hpp>
#include <boost/javascript/global.hpp>
#include <boost/javascript/reference.hpp>
#include <boost/javascript/array.hpp>
#include <boost/javascript/null.hpp>
#include <boost/javascript/date.hpp>
#include <boost/javascript/js_function.hpp>
#include <boost/javascript/js_object.hpp>
#include <boost/javascript/complex.hpp>
#include <boost/javascript/undefined.hpp>

#include <boost/clipp/container_wrapper.ipp>

using namespace boost::javascript;
using namespace boost::clipp;
using namespace boost::spirit;

//#define SPIRIT_DUMP_PARSETREE_AS_XML
#if BOOST_WORKAROUND( BOOST_MSVC, == 1300)
//'void boost::clipp::container_wrapper<T>::init(boost::clipp::context *)' : 
//no suitable definition provided for explicit template instantiation request
//since init is a static function, this is the intent.
#pragma warning (disable:4661)
#endif

typedef tree_match<javascript_parser::iterator_t,javascript_parser::factory_t, nil_t> parse_tree_match_t;
typedef parse_tree_match_t::tree_iterator iter_t;

valueP eval_expression(javascript_parser* parser_,iter_t const& i, callback_handler const& handler);
void eval_params(javascript_parser* parser_,iter_t const& i,value::Params& params, callback_handler const& handler);

class lookup_scope_class : public boost::clipp::value {
public:
    lookup_scope_class(std::list<boost::clipp::valueP>& stack) : stack_(stack) {}
    virtual valueP lookup(const std::string& identifier,valueP parent = nullptr);
private:
    std::list<boost::clipp::valueP>& stack_;
};

valueP lookup_scope_class::lookup(const std::string& identifier,valueP parent)
{
    for(std::list<boost::clipp::valueP>::reverse_iterator it=stack_.rbegin();it!=stack_.rend();++it) {
        boost::clipp::valueP stack_object=*it;
        valueP value=stack_object->lookup(identifier,parent);
        if(value) return value;
    }
    return nullptr;
}

void javascript_parser::top_stack_only(bool status)
{
    top_stack_only_=status;
}

bool javascript_parser::top_stack_only() const
{
    return top_stack_only_;
}

javascript_parser::javascript_parser()
    : m_handler(0)
{
    top_stack_only_=false;
    lookup_scope_ = new lookup_scope_class(lookup_scope_stack_);
    get_context()->add_type_name(typeid(value),"value");
    valueP global_=new boost::javascript::global;
    get_context()->setGlobal(global_);
    global::init(get_context());
    global_->create(get_context());

    PreferredType::init(get_context());
    number::init(get_context());
    complex::init(get_context());
    js_string::init(get_context());
    math::init(get_context());
    boolean::init(get_context());
    reference::init(get_context());
    array::init(get_context());
    null::init(get_context());
    date::init(get_context());
    js_function::init(get_context());
    js_object::init(get_context());
    push_all_scope(global_);
    get_context()->setNull(new boost::javascript::null);
    get_context()->null()->create(get_context());
}

javascript_parser::~javascript_parser()
{
}

valueP javascript_parser::parse(std::ifstream& file,callback_handler const& handler)
{
    if(!file) {
        handler.report_error("Could not open input file: \n");
        return nullptr;
    }

    file.unsetf(std::ios::skipws); //  Turn of white space skipping on the stream

    std::vector<char> vec;
    std::copy(
        std::istream_iterator<char>(file),
        std::istream_iterator<char>(),
        std::back_inserter(vec));

    return parse(&vec.front(),&vec.back()+1,handler);
}

valueP javascript_parser::parse(const char* str,callback_handler const& handler)
{
    char const* last = str;
    while (*last) last++;
    return parse(str,last,handler);
}
//#define SPIRIT_DUMP_PARSETREE_AS_XML

void javascript_parser::dump(tree_parse_t & info)
{
#if defined(SPIRIT_DUMP_PARSETREE_AS_XML)
        // dump parse tree as XML
        std::map<parser_id, std::string> rule_names;
        rule_names[identifierID] = "identifierID";
        rule_names[string_literalID] = "string_literalID";
        rule_names[numeric_literalID] = "numeric_literalID";
        rule_names[null_literalID] = "null_literalID";
        rule_names[boolean_literalID] = "boolean_literalID";
        rule_names[this_literalID] = "this_literalID";
        rule_names[postfix_expressionID] = "postfix_expressionID";
        rule_names[unary_expressionID] = "unary_expressionID";
        rule_names[multiplicative_expressionID] = "multiplicative_expressionID";
        rule_names[additive_expressionID] =         "additive_expressionID";
        rule_names[bitwise_shift_expressionID] =    "bitwise_shift_expressionID";
        rule_names[relational_expressionID] =       "relational_expressionID";
        rule_names[relational_expression_no_inID] = "relational_expression_no_inID";
        rule_names[equality_expressionID] =         "equality_expressionID";
        rule_names[equality_expression_no_inID] =   "equality_expression_no_inID";
        rule_names[bitwise_AND_expressionID] =      "bitwise_AND_expressionID";
        rule_names[bitwise_AND_expression_no_inID] ="bitwise_AND_expression_no_inID";
        rule_names[bitwise_XOR_expressionID] =      "bitwise_XOR_expressionID";
        rule_names[bitwise_XOR_expression_no_inID] ="bitwise_XOR_expression_no_inID";
        rule_names[bitwise_OR_expressionID] =       "bitwise_OR_expressionID";
        rule_names[bitwise_OR_expression_no_inID] = "bitwise_OR_expression_no_inID";
        rule_names[logical_AND_expressionID] =      "logical_AND_expressionID";
        rule_names[logical_AND_expression_no_inID] ="logical_AND_expression_no_inID";
        rule_names[logical_OR_expressionID] =       "logical_OR_expressionID";
        rule_names[logical_OR_expression_no_inID] = "logical_OR_expression_no_inID";
        rule_names[conditional_expressionID] =      "conditional_expressionID";
        rule_names[conditional_expression_no_inID] ="conditional_expression_no_inID";
        rule_names[assignment_expressionID] =       "assignment_expressionID";
        rule_names[assignment_expression_no_inID] = "assignment_expression_no_inID";
        rule_names[programID] = "programID";
        rule_names[left_hand_side_expressionID] = "left_hand_side_expressionID";
        rule_names[member_expressionID] = "member_expressionID";
        rule_names[new_expressionID] = "new_expressionID";
        rule_names[call_expressionID] = "call_expressionID";
        rule_names[argumentsID] = "argumentsID";
        rule_names[variable_statementID] = "variable_statementID";
        rule_names[variable_declarationID] = "variable_declarationID";
        rule_names[variable_declaration_no_inID] = "variable_declaration_no_inID";
        rule_names[empty_statementID] = "empty_statementID";
        rule_names[expression_statementID] = "expression_statementID";
        rule_names[variable_declaration_listID] = "variable_declaration_listID";
        rule_names[variable_declaration_list_no_inID] = "variable_declaration_list_no_inID";
        rule_names[if_statementID] = "if_statementID";
        rule_names[do_while_statementID] = "do_while_statementID";
        rule_names[while_statementID] = "while_statementID";
        rule_names[blockID] = "blockID";
        rule_names[for_condition_standardID] = "for_condition_standardID";
        rule_names[for_condition_inID] = "for_condition_inID";
        rule_names[function_declarationID] = "function_declarationID";
        rule_names[function_expressionID] = "function_expressionID";
        rule_names[formal_parameter_listID] = "formal_parameter_listID";
        rule_names[function_bodyID] = "function_bodyID";
        rule_names[source_elementID] = "source_elementID";
        rule_names[return_statementID] = "return_statementID";
        rule_names[continue_statementID] = "continue_statementID";
        rule_names[break_statementID] = "break_statementID";
        rule_names[with_statementID] = "with_statementID";
        rule_names[escape_sequenceID] = "escape_sequenceID";
        rule_names[hex_escape_sequenceID] = "hex_escape_sequenceID";
        rule_names[character_escape_sequenceID] = "character_escape_sequenceID";
        rule_names[null_escape_sequenceID] = "null_escape_sequenceID";
        rule_names[string_charactersID] = "string_charactersID";
        tree_to_xml(std::cout, info.trees, "expression", rule_names);
#endif
}

valueP javascript_parser::evaluate(tree_parse_t & info,callback_handler const& handler)
{
    try {
        if(!info.full) return nullptr;
        return eval_expression(this,info.trees.begin(), handler);
    }
    catch(js_exit_return&) {
        throw;
    }
    catch (positional_runtime_error& e) {
        handler.parser_pos() = e.parser_pos();
        if (handler.is_exception_handler()) throw;
        else {
            handler.report_error(e.what());
            return 0;
        }
    }
    catch(std::exception& e) {
        if(handler.is_exception_handler()) throw;
        else {
            handler.report_error(e.what());
            return nullptr;
        }
    }
}

void eval_params(javascript_parser* parser_,iter_t const& i,value::Params& params, callback_handler const& handler)
{
    for(iter_t it=i->children.begin();it!=i->children.end();++it) {
        params.push_back(eval_expression(parser_,it, handler));
    }
}

valueP eval_expression(javascript_parser* parser_,iter_t const& i, callback_handler const& handler)
{

#if defined(SPIRIT_DUMP_PARSETREE_AS_XML)
    using namespace boost::clipp::detail;
    std::cout << "In eval_expression. i->value = " <<
        std::string(i->value.begin(), i->value.end()) <<
        " i->children.size() = " << i->children.size() << std::endl;
#endif
    handler.parser_pos().set_current(i->value.begin());
    switch(i->value.id().to_long()) {
    case call_expressionID:
        {   
            valueP callee = eval_expression(parser_,i->children.begin(),handler);
            iter_t const& inner_i=i->children.begin()+1;
            switch(*inner_i->value.begin()) {
            case '(':
                value::Params params;
                eval_params(parser_,inner_i,params,handler);
                valueP result = callee->call(params);
                if (!result)
                {					
                    result = wrap(new undefined() ,parser_->get_context());
                }
                handler.branch_call();
                return result;
            }
            
            break;
        }
    case member_expressionID:
        {
            switch(*i->value.begin()) {
            case '[':
                return invoke_operator<'[]'>(
                    eval_expression(parser_,i->children.begin(),handler),
                    eval_expression(parser_,i->children.begin()+1,handler)
                );
            case '.': 
                {
                    scope_pusher_all push_scope(parser_,eval_expression(parser_,i->children.begin(),handler));
                    lookup_scope_top_stack lock_scope(parser_);
                    return eval_expression(parser_,i->children.begin()+1,handler);
                }
            case 'n': //new
                {
                    valueP callee = eval_expression(parser_,i->children.begin(),handler);
                    iter_t const& inner_i=i->children.begin()+1;
                    switch(*inner_i->value.begin()) {
                    case '(':
                        value::Params params;
                        eval_params(parser_,inner_i,params,handler);
                        valueP result = callee->construct(params);
                        if (!result)
                        {					
                            result = wrap(new undefined() ,parser_->get_context());
                        }
                        return result;
                    }
                }
            }
            break;
        }
    case new_expressionID:
        {
            valueP result = eval_expression(parser_,i->children.begin(),handler);
            return result;
/*            value::Params params;
            return result->construct(params);*/
        }
    case identifierID:
        {
            std::string name = std::string(i->value.begin(),i->value.end());
            valueP result = parser_->lookup_scope()->lookup(name,parser_->lookup_scope());
            if(!result) {
                result = parser_->declaration_scope()->insert(name,wrap(new reference,parser_->get_context()));
            }
            return result;
        }
        break;
    case string_literalID:
        {
            valueP result=wrap(std::string(),parser_->get_context());
            for(iter_t it = i->children.begin(); it!=i->children.end(); ++it) {
                if(result) result = invoke_operator<'+'>(result,eval_expression(parser_,it,handler));
                else result=eval_expression(parser_,it,handler);
            }
            return result;
        }
    case string_charactersID:
        {
            return wrap(std::string(i->value.begin(),i->value.end()),parser_->get_context());
        }
    case hex_escape_sequenceID:
        {
            unsigned char literal=0;            
            char value = *i->value.begin();
            if(value>='0' && value<='9') value-='0';
            else if(value>='a' && value<='f') value-='a'-10;
            else if(value>='A' && value<='F') value-='A'-10;
            literal=value<<5;
            value = *(i->value.begin()+1);
            if(value>='0' && value<='9') value-='0';
            else if(value>='a' && value<='f') value-='a'-10;
            else if(value>='A' && value<='F') value-='A'-10;
            literal+=value;            
            return wrap(std::string(1,literal),parser_->get_context());
        }
    case character_escape_sequenceID:
        {
            switch(*i->value.begin()) {
            case '\'': return wrap("\'",parser_->get_context());
            case '\"': return wrap("\"",parser_->get_context());
            case '\\': return wrap("\\",parser_->get_context());
            case 'b': return wrap("\b",parser_->get_context());
            case 'f': return wrap("\f",parser_->get_context());
            case 'n': return wrap("\n",parser_->get_context());
            case 'r': return wrap("\r",parser_->get_context());
            case 't': return wrap("\t",parser_->get_context());
            case 'v': return wrap("\v",parser_->get_context());
            default:return wrap(std::string(1,*i->value.begin()),parser_->get_context());
            }
        }
    case null_escape_sequenceID:
        return wrap(std::string(1,0),parser_->get_context());
    case numeric_literalID:
        {
            return wrap(i->value.value(),parser_->get_context());
        }
        break;
    case null_literalID:
        {
            return parser_->get_context()->null();
        }
        break;
    case boolean_literalID:
        {
            switch(*i->value.begin()) {
            case 't': //true;
                return wrap(true,parser_->get_context());
            case 'f': //false;
                return wrap(false,parser_->get_context());               
            }                
        }
        break;
    case this_literalID:
        {
            valueP result=parser_->lookup_scope()->lookup("this",parser_->lookup_scope());
            if(!result) result=parser_->global();
            return result;
        }
        break;
    case array_literalID:
        {
            size_t size = i->children.end() - i->children.begin();
            array* a = new array(size);
            valueP result = wrap(a,parser_->get_context());
            for(size_t counter=0;counter<size;++counter) {
                iter_t const& child=i->children.begin()+counter;
                if(*child->value.begin()!=',') {
                    (*a)[counter]=eval_expression(parser_,i->children.begin()+counter,handler);
                }
            }
            return result;            
            break;
        }
    case postfix_expressionID:
        {
            switch(*i->value.begin()) {
            case '+'://++
                return invoke_operator<'++'>(eval_expression(parser_,i->children.begin(),handler),int());
            case '-'://--
                return invoke_operator<'--'>(eval_expression(parser_,i->children.begin(),handler),int());
            }
        }
        break;
    case unary_expressionID:
        {
            switch(*i->value.begin()) {
            case 'd'://delete
                {
                    valueP object = eval_expression(parser_,i->children.begin(),handler);
                    parser_->declaration_scope()->erase(object);
                    return parser_->get_context()->null();
                }
            case 'v'://void
                break;
            case 't'://typeof
               {
                         valueP object = eval_expression(parser_,i->children.begin(),handler).get()->duplicate();
                         std::string rc = "undefined";
                         if (object)
                         {
                              rc = parser_->get_context()->type_name(object->type());
                              std::transform(rc.begin(), rc.end(), rc.begin(), tolower);
                              if (rc=="null") rc = "object";
                         }
                         return wrap(rc, parser_->get_context());				
                }
                break;
            case '+'://++ or +
                if(boost::next(i->value.begin())==i->value.end()) //-
                    return invoke_operator<'+'>(
                        eval_expression(parser_,i->children.begin(),handler)
                    );
                else //++
                    return invoke_operator<'++'>(
                        eval_expression(parser_,i->children.begin(),handler)
                    );
                break;
            case '-'://-- or -
                if(i->value.end() - i->value.begin() == 1) //-
                    return invoke_operator<'-'>(
                        eval_expression(parser_,i->children.begin(),handler)
                    );
                else //--
                    return invoke_operator<'--'>(
                        eval_expression(parser_,i->children.begin(),handler)
                    );
                break;
            case '~':
                return invoke_operator<'~'>(
                    eval_expression(parser_,i->children.begin(),handler)
                );
            case '!':
                return invoke_operator<'!'>(
                    eval_expression(parser_,i->children.begin(),handler)
                );
            }
        }
        break;
    case multiplicative_expressionID:
        {
            switch(*i->value.begin()) {
            case '*':
                return invoke_operator<'*'>(
                    eval_expression(parser_,i->children.begin(),handler),
                    eval_expression(parser_,i->children.begin()+1,handler)
                );
            case '/':
                return invoke_operator<'/'>(
                    eval_expression(parser_,i->children.begin(),handler),
                    eval_expression(parser_,i->children.begin()+1,handler)
                );
            case '%':
                return invoke_operator<'%'>(
                    eval_expression(parser_,i->children.begin(),handler),
                    eval_expression(parser_,i->children.begin()+1,handler)
                );
            }
        }
        break;
    case bitwise_shift_expressionID:
        {
            switch(*i->value.begin()) {
            case '<': //<
                return invoke_operator<'<<'>(
                    eval_expression(parser_,i->children.begin(),handler),
                    eval_expression(parser_,i->children.begin()+1,handler)
                );
            case '>': // >> or >>>
                if(i->value.end() - i->value.begin() == 2) // >>
                    return invoke_operator<'>>'>(
                        eval_expression(parser_,i->children.begin(),handler),
                        eval_expression(parser_,i->children.begin()+1,handler)
                    );
                else {
                    unsigned int lhs=unwrap<unsigned int>(eval_expression(parser_,i->children.begin(),handler))();
                    unsigned int rhs=unwrap<unsigned int>(eval_expression(parser_,i->children.begin()+1,handler))();
                    return wrap(lhs>>rhs,parser_->get_context());
                }                    
            }
        }
        break;
    case additive_expressionID:
        {
            switch(*i->value.begin()) {
            case '+':
                return invoke_operator<'+'>(
                    eval_expression(parser_,i->children.begin(),handler),
                    eval_expression(parser_,i->children.begin()+1,handler)
                );
            case '-':
                return invoke_operator<'-'>(
                    eval_expression(parser_,i->children.begin(),handler),
                    eval_expression(parser_,i->children.begin()+1,handler)
                );
            }
        }
        break;
    case relational_expressionID:
    case relational_expression_no_inID:
        {
            switch(*i->value.begin()) {
            case '>': // > or >=
                if(i->value.end() - i->value.begin() == 1) // >
                    return invoke_operator<'>'>(
                        eval_expression(parser_,i->children.begin(),handler),
                        eval_expression(parser_,i->children.begin()+1,handler)
                    );
                else // >=
                    return invoke_operator<'>='>(
                        eval_expression(parser_,i->children.begin(),handler),
                        eval_expression(parser_,i->children.begin()+1,handler)
                    );
            case '<':
                if(i->value.end() - i->value.begin() == 1) // <
                    return invoke_operator<'<'>(
                        eval_expression(parser_,i->children.begin(),handler),
                        eval_expression(parser_,i->children.begin()+1,handler)
                    );
                else // <=
                    return invoke_operator<'<='>(
                        eval_expression(parser_,i->children.begin(),handler),
                        eval_expression(parser_,i->children.begin()+1,handler)
                    );
            case 'i': // in or instanceof
                if(i->value.end() - i->value.begin() == 2) //in
                {
                    valueP property=eval_expression(parser_,i->children.begin(),handler);
                    valueP object=eval_expression(parser_,i->children.begin()+1,handler);
                    if(!object) return wrap(false,parser_);
                    unwrap<std::string> name(property);
                    if(!name.ok()) return wrap(false,parser_);
                    return wrap_struct<bool>::wrap(object->lookup(name()).get()!=0,parser_);                    
                }
                else //instanceof 
                {
                    valueP object1 = eval_expression(parser_,i->children.begin(),handler);
                    valueP object2 = eval_expression(parser_,i->children.begin()+1,handler);
                    unwrap<js_function *> function(object2);
                    unwrap<js_object *> object(object1);
                    if(object.ok() && function.ok()) 
                    {
                        valueP ctor = object()->lookup("constructor");
                        unwrap<js_function *> object_fn(ctor);
                        if (object_fn.ok())
                        {
                            if(object_fn()==function())
                            {
                                return wrap(true, parser_->get_context());
                            }
                        }
                    }
                    unwrap<value *> helper2(object2);
                    unwrap<value *> helper1(object1);
                    if (helper1.ok() && helper2.ok())
                    {
                        if (helper2()->type() == typeid(member_table))
                        {
                            member_table *mt = dynamic_cast<member_table *>(helper2());
                            if (mt)
                            {
                                return wrap(helper1()->type() == mt->info(), parser_->get_context());
                            }
                        }
                    }					
                    return wrap(false, parser_->get_context());
                }
            }
        }
        break;
    case equality_expressionID:
    case equality_expression_no_inID:
        {
            switch(*i->value.begin()) {
            case '=': // == or ===
                if(i->value.end() - i->value.begin() == 2) // ==
                    return invoke_operator<'=='>(
                        eval_expression(parser_,i->children.begin(),handler),
                        eval_expression(parser_,i->children.begin()+1,handler)
                    );
                else {
                    valueP lhs=eval_expression(parser_,i->children.begin(),handler);
                    valueP rhs=eval_expression(parser_,i->children.begin()+1,handler);
                    if(!lhs && !rhs) return wrap(true,parser_->get_context());
                    if(lhs && rhs) {
                        return wrap(lhs->duplicate()==rhs->duplicate(),parser_->get_context());
                    }
                    return wrap(false,parser_->get_context());
                }
            case '!': // != or !==
                if(i->value.end() - i->value.begin() == 2) // !=
                    return invoke_operator<'!='>(
                        eval_expression(parser_,i->children.begin(),handler),
                        eval_expression(parser_,i->children.begin()+1,handler)
                    );
                else {
                    valueP lhs=eval_expression(parser_,i->children.begin(),handler);
                    valueP rhs=eval_expression(parser_,i->children.begin()+1,handler);
                    if(!lhs && !rhs) return wrap(false,parser_->get_context());
                    if(lhs && rhs) {
                        return wrap(lhs->duplicate()!=rhs->duplicate(),parser_->get_context());
                    }
                    return wrap(true,parser_->get_context());
                }
            }
        }
        break;
    case bitwise_AND_expressionID:
    case bitwise_AND_expression_no_inID:
        return invoke_operator<'&'>(
            eval_expression(parser_,i->children.begin(),handler),
            eval_expression(parser_,i->children.begin()+1,handler)
        );            
    case bitwise_XOR_expressionID:
    case bitwise_XOR_expression_no_inID:
        return invoke_operator<'^'>(
            eval_expression(parser_,i->children.begin(),handler),
            eval_expression(parser_,i->children.begin()+1,handler)
        );            
    case bitwise_OR_expressionID:
    case bitwise_OR_expression_no_inID:
        return invoke_operator<'|'>(
            eval_expression(parser_,i->children.begin(),handler),
            eval_expression(parser_,i->children.begin()+1,handler)
        );            
    case logical_AND_expressionID:
    case logical_AND_expression_no_inID:
        if(!unwrap<bool>(eval_expression(parser_,i->children.begin(),handler))()) return wrap(false,parser_->get_context());
        else return wrap(unwrap<bool>(eval_expression(parser_,i->children.begin()+1,handler))(),parser_->get_context());
    case logical_OR_expressionID:
    case logical_OR_expression_no_inID:
        if(unwrap<bool>(eval_expression(parser_,i->children.begin(),handler))()) return wrap(true,parser_->get_context());
        else return wrap(unwrap<bool>(eval_expression(parser_,i->children.begin()+1,handler))(),parser_->get_context());
    case conditional_expressionID:
    case conditional_expression_no_inID:
        if(unwrap<bool>(eval_expression(parser_,i->children.begin(),handler))())
            return eval_expression(parser_,i->children.begin()+1,handler);
        else 
            return eval_expression(parser_,i->children.begin()+2,handler);
    case assignment_expressionID:
    case assignment_expression_no_inID:
        {
            switch(*i->value.begin()) {
            case '=':
                return invoke_operator<'='>(
                    eval_expression(parser_,i->children.begin(),handler),
                    eval_expression(parser_,i->children.begin()+1,handler)
                );
            case '*': // *=
                return invoke_operator<'*='>(
                    eval_expression(parser_,i->children.begin(),handler),
                    eval_expression(parser_,i->children.begin()+1,handler)
                );
            case '/': // /=
                return invoke_operator<'/='>(
                    eval_expression(parser_,i->children.begin(),handler),
                    eval_expression(parser_,i->children.begin()+1,handler)
                );
            case '%': // %=
                return invoke_operator<'%='>(
                    eval_expression(parser_,i->children.begin(),handler),
                    eval_expression(parser_,i->children.begin()+1,handler)
                );
            case '+': // +=
                return invoke_operator<'+='>(
                    eval_expression(parser_,i->children.begin(),handler),
                    eval_expression(parser_,i->children.begin()+1,handler)
                );
            case '-': // -=
                return invoke_operator<'-='>(
                    eval_expression(parser_,i->children.begin(),handler),
                    eval_expression(parser_,i->children.begin()+1,handler)
                );
            case '<': // <<=
                return invoke_operator<'<<='>(
                    eval_expression(parser_,i->children.begin(),handler),
                    eval_expression(parser_,i->children.begin()+1,handler)
                );
            case '>': // >>= or >>>=
                if(i->value.end() - i->value.begin() == 3) // >>=
                    return invoke_operator<'>>='>(
                        eval_expression(parser_,i->children.begin(),handler),
                        eval_expression(parser_,i->children.begin()+1,handler)
                    );
                else {
                    valueP lhs=eval_expression(parser_,i->children.begin(),handler);
                    unsigned int rhs=unwrap<unsigned int>(eval_expression(parser_,i->children.begin()+1,handler))();
                    unsigned int result=unwrap<unsigned int>(lhs)()>>rhs;
                    invoke_operator<'='>(lhs,wrap(result,parser_->get_context()));
                }
            case '&': // &=
                return invoke_operator<'&='>(
                    eval_expression(parser_,i->children.begin(),handler),
                    eval_expression(parser_,i->children.begin()+1,handler)
                );
            case '^': // ^=
                return invoke_operator<'^='>(
                    eval_expression(parser_,i->children.begin(),handler),
                    eval_expression(parser_,i->children.begin()+1,handler)
                );
            case '|': // |=
                return invoke_operator<'|='>(
                    eval_expression(parser_,i->children.begin(),handler),
                    eval_expression(parser_,i->children.begin()+1,handler)
                );
            ;
            }
        }
        break;
    case variable_declarationID:
    case variable_declaration_no_inID:
        {
            valueP rhs = eval_expression(parser_,i->children.begin()+1,handler);
            lookup_scope_top_stack lock_scope(parser_);
            valueP lhs = eval_expression(parser_,i->children.begin(),handler);
            return invoke_operator<'='>(
                lhs,
                rhs
            );
        }
        break;
    case variable_declaration_listID:
    case variable_declaration_list_no_inID:
        {
            for(iter_t it = i->children.begin(); it!=i->children.end(); ++it) {
                eval_expression(parser_,it,handler);
            }
        }
        break;
    case if_statementID:
        if(unwrap<bool>(eval_expression(parser_,i->children.begin(),handler))()) {
            return eval_expression(parser_,i->children.begin()+1,handler);
        }
        else if(i->children.end() - i->children.begin() == 3) {
            return eval_expression(parser_,i->children.begin()+2,handler);
        }
        break;
    case variable_statementID:
        {
            for(iter_t it = i->children.begin(); it!=i->children.end(); ++it) {
                eval_expression(parser_,it,handler);
            }
            return nullptr;
        }
    case statement_listID:
        {
            valueP result;
            for(iter_t it = i->children.begin(); it!=i->children.end(); ++it) {
                result=eval_expression(parser_,it,handler);
            }
            return result;
        }
    case blockID:
        {
            if(i->children.size()==0) return 0;
            return eval_expression(parser_,i->children.begin(),handler);
        }
    case do_while_statementID:
        {
            valueP result;
            do {
                handler.branch_call();
                try {
                    result=eval_expression(parser_,i->children.begin(),handler);
                }
                catch(js_continue&) {
                    continue;
                }
                catch(js_break&) {
                    break;
                }
            } while(unwrap<bool>(eval_expression(parser_,i->children.begin()+1,handler))());
            return result;
        }
        break;
    case while_statementID:
        {
            valueP result;
            while(unwrap<bool>(eval_expression(parser_,i->children.begin(),handler))()) {
                handler.branch_call();
                try {
                    result=eval_expression(parser_,i->children.begin()+1,handler);
                }
                catch(js_continue&) {
                    continue;
                }
                catch(js_break&) {
                    break;
                }
            }
            return result;
        }
        break;
    case for_statementID:
        {
            iter_t condition = i->children.begin();
            switch(condition->value.id().to_long()) {
            case for_condition_standardID:
                {
                    valueP result;
                    eval_expression(parser_,condition->children.begin(),handler);
                    iter_t const& conditional = condition->children.begin()+1;
                    //Support for(;;) ...
                    bool no_conditional=(conditional->value.id().to_long()==empty_statementID);
                    for(eval_expression(parser_,condition->children.begin(),handler);
                        no_conditional?true:unwrap<bool>(eval_expression(parser_,conditional,handler))();)
                    {
                        handler.branch_call();
                        try {
                            result = eval_expression(parser_,i->children.begin()+1,handler);
                        }
                        catch(js_continue&) {
                            if(condition->children.end()-condition->children.begin()==3) {
                                //counter
                                eval_expression(parser_,condition->children.begin()+2,handler);
                            }
                            continue;
                        }
                        catch(js_break&) {
                            break;
                        }
                        if(condition->children.end()-condition->children.begin()==3) {
                            //counter
                            eval_expression(parser_,condition->children.begin()+2,handler);
                        }
                    }
                    return result;
                }
            case for_condition_inID:
                {                    
                    valueP result;
                    valueP counter=eval_expression(parser_,condition->children.begin(),handler);
                    valueP in_expression=eval_expression(parser_,condition->children.begin()+1,handler);
                    if(!in_expression || in_expression->size()==0) return result;
                    for(value_iterator vit=in_expression->begin();vit!=in_expression->end();++vit) {
                        handler.branch_call();
                        try {
                            invoke_operator<'='>(counter,*vit);
                            result = eval_expression(parser_,i->children.begin()+1,handler);
                        }
                        catch(js_continue&) {
                            continue;
                        }
                        catch(js_break&) {
                            break;
                        }
                    }
                    return result;
                }
            }
            break;
        }
    case function_declarationID:
        {
            //Extract argument string
            iter_t it = i->children.begin()+1;
            std::string arguments;
            if(it->children.begin()!=it->children.end()) {
                iter_t istart=it->children.begin();
                iter_t iend  =boost::prior(it->children.end());
                arguments=std::string(istart->value.begin(),iend->value.end());
            }
            //Extract program string
            it=i->children.begin()+2;
            iter_t istart=it->children.begin();
            iter_t iend=boost::prior(it->children.end());
            std::string program(istart->value.begin()+1,iend->value.begin());
            handler.parser_pos().set_current(istart->value.begin() + 1);
            valueP function = wrap(new js_function(arguments, program, handler), parser_->get_context());
            invoke_operator<'='>(eval_expression(parser_,i->children.begin(),handler),function);
            break;
        }
    case function_expressionID:
        {
            //Simple case, unnamed function
            if(i->children.end()-i->children.begin()==2) {
                iter_t it = i->children.begin();    
                std::string arguments;
                if(it->children.begin()!=it->children.end()) {
                    iter_t istart=it->children.begin();
                    iter_t iend  =boost::prior(it->children.end());
                    arguments=std::string(istart->value.begin(),iend->value.end());
                }
                it=i->children.begin()+1;
                iter_t istart=it->children.begin();
                iter_t iend=boost::prior(it->children.end());
                std::string program(istart->value.begin()+1,iend->value.begin());
                return wrap(new js_function(arguments,program),parser_->get_context());
            }
            //Complex case, named function
            else {
                valueP object = wrap(new js_object,parser_->get_context());
                scope_pusher_all push_scope(parser_,object);

                //Extract argument string
                iter_t it = i->children.begin()+1;
                std::string arguments;
                if(it->children.begin()!=it->children.end()) {
                    iter_t istart=it->children.begin();
                    iter_t iend  =boost::prior(it->children.end());
                    arguments=std::string(istart->value.begin(),iend->value.end());
                }
                //Extract program string
                it=i->children.begin()+2;
                iter_t istart=boost::next(it->children.begin());
                iter_t iend=boost::prior(it->children.end());
                std::string program(istart->value.begin(),iend->value.begin());

                valueP function = wrap(new js_function(arguments,program),parser_->get_context());
                return invoke_operator<'='>(eval_expression(parser_,i->children.begin(),handler),function);
            }
            break;
        }
    case return_statementID:
        {
            if (i->children.begin() !=i->children.end())
            {
                throw js_exit_return(eval_expression(parser_,i->children.begin(),handler));
            }
            else
            {
                throw js_exit_return(new undefined());
            }
        }
    case continue_statementID:
        {
            throw js_continue();
        }
    case break_statementID:
        {
            throw js_break();
        }
    case programID:
        {
            valueP result;
            for(iter_t it = i->children.begin(); it!=i->children.end(); ++it) {
                result=eval_expression(parser_,it,handler);
            }
            return result;
        }
        break;
    case empty_statementID:
        {
            return nullptr;
        }
    case with_statementID:
        {
            scope_pusher_all push_scope(parser_,eval_expression(parser_,i->children.begin(),handler));
            return eval_expression(parser_,i->children.begin()+1,handler);
        }
    case switch_statementID:
        {
            valueP expression = eval_expression(parser_,i->children.begin(),handler);
            int default_index=-1;
            int case_size=(i->children.end()-i->children.begin())-1;
            for(int case_i=1;case_i<=case_size;++case_i) {
                iter_t it=i->children.begin()+case_i;
                switch(it->value.id().to_long()) {
                case default_clauseID: 
                    default_index=case_i;
                    break;
                case case_clauseID:
                    if(unwrap<bool>(invoke_operator<'=='>(eval_expression(parser_,it->children.begin(),handler),expression))()) {
                        return eval_expression(parser_,it->children.begin()+1,handler);
                    }
                    break;
                }
            }
            if(default_index!=-1) {
                iter_t it=i->children.begin()+default_index;
                return eval_expression(parser_,it->children.begin(),handler);
            }
            break;
        }
    case throw_statementID:
        {
            valueP expression = eval_expression(parser_,i->children.begin(),handler);
            throw js_throw_statement(expression);
        }
    case try_statementID:
        {
            valueP expression;
            int catch_index=-1;
            int finally_index=-1;
            int size=(i->children.end()-i->children.begin())-1;
            if(size==2) {
                catch_index=1;
                finally_index=2;
            }
            else if(size==1) {
                iter_t it=i->children.begin()+1;
                if(it->value.id().to_long()==catch_clauseID) catch_index=1;
                else finally_index=1;
            }
            try {
                expression = eval_expression(parser_,i->children.begin(),handler);
            }
            catch(js_throw_statement& e) {
                if(catch_index>=0) {
                    iter_t it=i->children.begin()+catch_index;
                    std::string name = std::string(it->children.begin()->value.begin(),it->children.begin()->value.end());
                    valueP object = wrap(new js_object,parser_->get_context());
                    object->insert(name,e.expression());
                    scope_pusher_all push_scope(parser_,object);
                    expression = eval_expression(parser_,it->children.begin()+1,handler);
                }
                else if(finally_index>=0) {
                    iter_t it=i->children.begin()+finally_index;
                    return eval_expression(parser_,it->children.begin(),handler);
                }
            }
            if(finally_index) {
                iter_t it=i->children.begin()+finally_index;
                eval_expression(parser_,it->children.begin(),handler);
            }
            return expression;
        }
    default:
        int a=i->value.id().to_long();
        break;
    }


    return 0;
}

void javascript_parser::push_all_scope(boost::clipp::valueP value) 
{
    if(!value->created()) value->create(get_context());
    boost::clipp::valueP result=value;
    if(!lookup_scope_stack_.size()==0) {
        result = value->duplicate(declaration_scope());
        if(!result) result=value;
    }
    lookup_scope_stack_.push_back(result);
    declaration_scope_stack_.push(result);
}

void javascript_parser::push_lookup_scope(boost::clipp::valueP value)
{
    if(!value->created()) value->create(get_context());
    if(!lookup_scope_stack_.size()==0) {
        value = value->duplicate(declaration_scope());
    }
    lookup_scope_stack_.push_back(value);
}

void javascript_parser::pop_all_scope()
{
    lookup_scope_stack_.pop_back();
    declaration_scope_stack_.pop();
}

void javascript_parser::pop_lookup_scope()
{
    lookup_scope_stack_.pop_back();
}

valueP javascript_parser::declaration_scope() 
{
    return declaration_scope_stack_.top();
}

valueP javascript_parser::lookup_scope() 
{
    if(top_stack_only()) return lookup_scope_stack_.back();
    else return lookup_scope_;
}

