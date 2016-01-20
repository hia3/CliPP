#include <boost/javascript/complex.hpp>
#include <boost/clipp/class.hpp>
#include <boost/lexical_cast.hpp>
#include <complex>

using namespace boost::clipp;

namespace boost {namespace javascript {namespace complex {
    std::string toString(const std::complex<double>& arg) {
        return lexical_cast<std::string>(arg.real()) + " + " + lexical_cast<std::string>(arg.imag()) + "i";
    }
}}}

void boost::javascript::complex::init(clipp::context* c)
{
    class_<std::complex<double>,direct_storage_policy> cls("Complex",c);
    cls[constructor<const double&,const double&>()];
//    cls[self=double()];
//    cls[self=std::complex<double>()];
    cls[self+=double()];
    cls[self+=std::complex<double>()];
    cls[self-=double()];
    cls[self-=std::complex<double>()];
    cls[self*=double()];
    cls[self*=std::complex<double>()];
    cls[self/=double()];
    cls[self/=std::complex<double>()];
    cls[self+double()];
    cls[double()+self];
    cls[self+self];
    cls[self-double()];
    cls[double()-self];
    cls[self-self];
    cls[self*double()];
    cls[double()*self];
    cls[self*self];
    cls[self/double()];
    cls[double()/self];
    cls[self/self];
    cls[self==double()];
    cls[double()==self];
    cls[self==self];
    cls[self!=double()];
    cls[double()!=self];
    cls[self!=self];
    typedef double (std::complex<double>::*ptr_read)() const;
    cls.read("real",ptr_read(&std::complex<double>::real));
    cls.read("imag",ptr_read(&std::complex<double>::imag));
    cls.function("toString",toString);
}
