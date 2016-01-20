#include "boost/spirit/core.hpp"
#include "boost/spirit/tree/ast.hpp"
#include <boost/javascript/javascript_parser.hpp>
#include <boost/javascript/global.hpp>
#include <boost/clipp/class.hpp>
#include <boost/javascript/math.hpp>
#include <boost/javascript/undefined.hpp>

using namespace boost::javascript;
using namespace boost::clipp;
using namespace boost::spirit;
template <typename T = unsigned>
struct radix_parser : parser<radix_parser<T> >
{
    typedef radix_parser self_t;
    typedef parser<radix_parser<T> > base_t;

    template <typename ScannerT>
    struct result
    {
        typedef typename match_result<ScannerT, T>::type type;
    };

    radix_parser(int rad) : radix(rad) {}

    template <typename ScannerT>
    typename parser_result<self_t, ScannerT>::type
    parse(ScannerT const& scan) const
    {
        switch(radix) {
            case 2:return uint_parser<T, 2>().parse(scan);
            case 8:return uint_parser<T, 8>().parse(scan);
            case 10:return uint_parser<T, 10>().parse(scan);
            case 16:return uint_parser<T, 16>().parse(scan);
            default:return uint_parser<T, 10>().parse(scan);
        }
    }
private:
    int radix;
};

void global::init(context* c)
{
    if(is_type_defined<global>(c)) return;
    class_<global,object> cls(c);
    cls.read("NaN",math::NaN);
    cls.read("Infinity",math::Infinity);
    cls.read("undefined",new undefined);
    cls.read("Math",new math);
    cls.function("eval",&global::eval);
    cls.static_function("isNaN",math::is_NaN);
    cls.static_function("isFinite",math::is_finite);
    cls.static_function("parseInt",parseInt).signature("number",arg("radix")=10);
    cls.static_function("parseFloat",parseFloat);
}

valueP global::eval(const std::string& x)
{
    javascript_parser* parser=dynamic_cast<javascript_parser*>(get_context());
    if(!parser) throw std::runtime_error("invalid context");
    try {
        return parser->parse(x,exception_handler());
    }
    catch(js_exit_return& e) {
        return e.return_value();
    }
    catch(js_continue& e) {
        throw std::runtime_error(e.what());
    }
    catch(js_break& e) {
        throw std::runtime_error(e.what());
    }
    return nullptr;
}

static int static_sign;
static double static_value;

void flip_sign(char val)
{
    static_sign=-static_sign;
}
void reset_int(char val)
{
    static_value=0;
}
void read_int(unsigned value) 
{
    static_value=value;
}
void read_double(double value) 
{
    static_value=value;
}

double global::parseInt(const std::string& number,int radix) 
{
    static_sign=1;
    static_value=math::NaN();

    radix_parser<> radix_p(radix);
    parse(number.c_str(),
          *space_p >> *(ch_p('-')[&flip_sign] | ch_p('+')) >>  //sign
          ((str_p("0x") >> hex_p[&read_int]) | //forced hex
          (ch_p('0')[&reset_int] >> oct_p[&read_int]) | //forced oct
          (radix_p[&read_int])), //use radix to decide
          nothing_p
        );

    return static_sign*static_value;
}

double global::parseFloat(const std::string& number)
{
    static_value=math::NaN();
    parse(number.c_str(),
          *space_p >> real_p[&read_double],
          nothing_p
         );
    return static_value;
}
