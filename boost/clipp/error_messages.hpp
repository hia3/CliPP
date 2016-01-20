// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_ERROR_MESSAGES_HPP_HOLT_03082003
#define BOOST_CLIPP_ERROR_MESSAGES_HPP_HOLT_03082003

#include <boost/clipp/config.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/clipp/type_id.hpp>
#include <exception>


namespace boost { namespace clipp {
    class context;

    class BOOST_CLIPP_EXPORT_IMPORT scripting_exception : public std::exception {
public:
    scripting_exception(const std::string& error) : error_(error) {}
    scripting_exception() {}
    virtual ~scripting_exception() throw() {}

    void set_error(const std::string& error) const {error_=error;}
    virtual const char* what() const throw() {return error_.c_str();}
private:
    mutable std::string error_;
};

class BOOST_CLIPP_EXPORT_IMPORT bad_type_conversion : public scripting_exception {
public:
    bad_type_conversion(const std::string& from,const std::string& to) : from_(from),to_(to) {}
    bad_type_conversion(context* c,const type_detail& from,const type_detail& to);
    virtual ~bad_type_conversion() throw() {}

    virtual const char* what() const throw() {
        set_error("cannot convert" 
                  " from \'" + from_ +"\'"
                  " to \'" + to_ + "\'");
        return scripting_exception::what();
    }           

    const std::string& from() const {return from_;}
    const std::string& to() const {return to_;}
private:
    std::string from_;
    std::string to_;
};


class BOOST_CLIPP_EXPORT_IMPORT invalid_argument : public scripting_exception {
public:
    invalid_argument(bad_type_conversion const& e,unsigned int iarg) : from_(e.from()),to_(e.to()) , iarg_(iarg) {}
    invalid_argument(context* c,const std::type_info& from,const std::type_info& to,unsigned int iarg);
    virtual ~invalid_argument() throw() {}
    virtual const char* what() const throw() {
        set_error("cannot convert parameter " + 
                  lexical_cast<std::string>(iarg_) + 
                  " from \'" + from_ +"\'"
                  " to \'" + to_ + "\'");
        return scripting_exception::what();
    }
private:
    std::string from_;
    std::string to_;
    unsigned int iarg_;
};

class BOOST_CLIPP_EXPORT_IMPORT invalid_number_of_arguments : public scripting_exception {
public:
    invalid_number_of_arguments(int arity,int nargs,bool valarray=false) : arity_(arity) , nargs_(nargs) , valarray_(valarray) {}
    virtual  ~invalid_number_of_arguments() throw() {}
    virtual const char* what() const throw() {
        set_error("invalid number of arguments " + 
                  lexical_cast<std::string>(nargs_));
        return scripting_exception::what();
    }
private:
    int arity_;
    int nargs_;
    bool valarray_;
};

}} // namespace boost::clipp

#endif // BOOST_CLIPP_ERROR_MESSAGES_HPP_HOLT_03082003
