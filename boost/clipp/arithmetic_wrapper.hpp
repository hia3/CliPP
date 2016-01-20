// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_ARITHMETIC_WRAPPER_HPP_HOLT_25082003
#define BOOST_ARITHMETIC_WRAPPER_HPP_HOLT_25082003

#include "boost/clipp/object.hpp"

#include <list>

namespace boost { namespace clipp {

class BOOST_CLIPP_EXPORT_IMPORT arithmetic_wrapper_base : public object
{
public:
    static void init(context* c);  
};

template<typename T>
class BOOST_CLIPP_EXPORT_IMPORT arithmetic_wrapper : public arithmetic_wrapper_base
{
public: //external
    arithmetic_wrapper(const T& t) :t_(t) {}
    operator T&() {return t_;}
    operator bool() {return t_;}
    operator char() {return t_;}
    operator unsigned char() {return t_;}
    operator signed char() {return t_;}
    operator unsigned short() {return t_;}
    operator short() {return t_;}
    operator unsigned int() {return t_;}
    operator int() {return t_;}
    operator unsigned long() {return t_;}
    operator long() {return t_;}
    operator float() {return t_;}
    operator double() {return t_;}
    operator long double() {return t_;}

public: //internal
    static void init(context* c);  
private:
    T t_;
};

template<typename T>
void arithmetic_wrapper<T>::init(context* c)
{
    class_<arithmetic_wrapper<T>,arithmetic_wrapper_base,T> cls(typeid(T).name(),c);
    cls.converter(arguments<const T&>());
    cls.converter(operator T&);
    cls.converter(operator bool);
    cls.converter(operator char);
    cls.converter(operator unsigned char);
    cls.converter(operator signed char);
    cls.converter(operator unsigned short);
    cls.converter(operator short);
    cls.converter(operator unsigned int);
    cls.converter(operator int);
    cls.converter(operator unsigned long);
    cls.converter(operator long);
    cls.converter(operator float);
    cls.converter(operator double);
    cls.converter(operator long double);
}

}} //namespace boost::clipp

#endif //BOOST_ARITHMETIC_WRAPPER_HPP_HOLT_25082003