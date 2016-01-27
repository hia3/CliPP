#include <boost/spirit/include/classic_core.hpp>
#include <boost/spirit/include/classic_ast.hpp>
#include <boost/javascript/javascript_grammar.hpp>
#include <boost/clipp/context.hpp>
#include <boost/clipp/scope.hpp>
#include <boost/clipp/member_wrapper.hpp>
#include <boost/javascript/js_function.hpp>
#include <boost/javascript/js_object.hpp>
#include <boost/javascript/undefined.hpp>
#include <string>

using namespace boost::javascript;
using namespace boost::clipp;

typedef boost::spirit::classic::tree_match<javascript_parser::iterator_t,javascript_parser::factory_t, boost::spirit::classic::nil_t> parse_tree_match_t;
typedef parse_tree_match_t::tree_iterator iter_t;

namespace boost {namespace javascript {
    inline const char* begin_string(const std::string& str) {
        return str.c_str();
    }
    inline const char* end_string(const std::string& str) {
        return str.c_str()+str.length();
    }
    
struct function_info
{
    function_info(const std::string& args,const std::string& program);

    typedef javascript_parser::tree_parse_t tree_parse_t;
    typedef javascript_parser::iterator_t iterator_t;

    javascript_grammar grammar_;
    skip_grammar skip_;
    javascript_parser::tree_parse_t program_execution_;
    std::vector<std::string> identifiers_;
};

function_info::function_info(const std::string& args,const std::string& program)
:   program_execution_(ast_parse<javascript_parser::factory_t>(begin_string(program), end_string(program), grammar_, skip_))
{
    javascript_grammar::definition<javascript_grammar::scanner_t> detail_grammar(grammar_);
    javascript_parser::tree_parse_t arguments_execution_ = ast_parse<javascript_parser::factory_t>(begin_string(args), end_string(args), detail_grammar.formal_parameter_list, skip_);

    if(!arguments_execution_.full) throw std::runtime_error("Argument list not parseable");
    if(!program_execution_.full) throw std::runtime_error("Program body not parseable");

    iter_t const& i=arguments_execution_.trees.begin();
    size_t size = i->children.end() - i->children.begin();
    identifiers_.resize(size);
    for(size_t counter=0;counter<size;++counter) {
        iter_t const& child=i->children.begin()+counter;
        identifiers_[counter]=std::string(child->value.begin(),child->value.end());
    }
}

}} //namespace boost::javascript

js_function::js_function(std::valarray<std::string>& args)
:   function_info_(NULL)
{
    if(args.size()>0) {
        int narguments=args.size()-1;
        if(narguments>0) {
            arguments_=args[0];
            for(int i=1;i<narguments;++i) arguments_+=","+args[i];
        }
        program_=args[narguments];
    }
    arguments_="("+arguments_+")";

    //Initialize error handling utility (Can only handle the internal line number of this function)
    
    parser_pos_.set_range(program_.c_str(),program_.c_str()+program_.length());

    function_info_=new function_info(arguments_,program_);
}

js_function::js_function(const std::string& args,const std::string& program,const callback_handler& handler)
:   function_info_(NULL)
,   arguments_("("+args+")")
,   program_(program)
{
    //Initialize error handling utility (in order to localize the exact line where the error occured)
    parser_pos_.set_file_name(handler.parser_pos().file_name());
    int current_line=handler.parser_pos().get_current_line();
    if(current_line>0) parser_pos_.set_start_line(current_line);
    parser_pos_.set_range(program_.c_str(),program_.c_str()+program_.length());

    function_info_=new function_info(arguments_,program_);    
}

js_function::~js_function()
{
    delete function_info_;
}

valueP js_function::call(Params& args,valueP parent)
{
    if(!function_info_) function_info_=new function_info(arguments_,program_);
    javascript_parser* parser=dynamic_cast<javascript_parser*>(get_context());
    if(!parser) throw std::runtime_error("invalid context");
    valueP newScope=new scope;
    if(parent) newScope->insert("this",parent->duplicate());
    scope_pusher_all push(parser,newScope);
    for(unsigned i=0;i<function_info_->identifiers_.size();++i) {
        if(i<args.size()) parser->declaration_scope()->insert(function_info_->identifiers_[i],args[i]);
        else parser->declaration_scope()->insert(function_info_->identifiers_[i],wrap(new undefined,get_context()));
    }
    exception_handler handler;
    const callback_handler* used_handler;
    used_handler=parser->get_callback_handler();
    if(!used_handler) used_handler=&handler;
    try {        
        used_handler->parser_pos()=parser_pos_;
        parser->evaluate(function_info_->program_execution_,*used_handler);
    }
    catch(js_exit_return& e) {
        return e.return_value();
    }
    catch(js_continue& e) {
        throw positional_runtime_error(e.what(),used_handler->parser_pos());
    }
    catch(js_break& e) {
        throw positional_runtime_error(e.what(),used_handler->parser_pos());
    }
    catch(std::exception& e) {
        throw positional_runtime_error(e.what(),used_handler->parser_pos());
    }
    return NULL;
}

valueP js_function::construct(Params& args,valueP parent)
{
    valueP result=wrap(new js_object(),get_context());
    result->insert("constructor", this);
    call(args,result);
    return result;
}

std::string js_function::toString() const
{
    return "function"+arguments_+"{\n" + program_ + "\n}\n";
}

valueP js_function::wrap_value(valueP parent)
{
   if(unwrap<js_object*>(parent).ok()) {
      valueP result=new member_wrapper(this,parent);
      result->create(get_context());
      return result;
   }
   else return this;
}
