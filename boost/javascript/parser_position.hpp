// Copyright Jaap de Wolff 2005
// Copyright Peder Holt 2007. 
// Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_JAVASCRIPT_PARSER_POSITION_HPP_HOLT_04072007
#define BOOST_JAVASCRIPT_PARSER_POSITION_HPP_HOLT_04072007

#include <string>
#include <boost/javascript/config.hpp>

namespace boost { namespace javascript {

class BOOST_JAVASCRIPT_EXPORT_IMPORT parser_position {
public:
    parser_position (const char* first, const char* last, const std::string &file_name = "", int start_line = 1);
    parser_position ();
    parser_position (const parser_position& other);
    parser_position& operator=(const parser_position& other);
    void set_range(const char* first,const char* last);
    void set_file_name(const std::string& file_name);
    void set_start_line(int start_line);
    bool set_current(const char* current); //!< returns true if valid position, else returns false;
    int get_current_line() const;
    int get_line_at(const char* current) const;
    std::string get_line_message(const std::string &pos_message) const;
    const std::string &file_name() const {return file_name_;}
    const char* start() const {return first_;}
protected:
    int get_line_at(const char* current, const char *&line_start) const;
private:
    int start_line_;
    const char* first_;
    const char* last_;
    const char* current_;
    std::string file_name_;
};

}} //namespace boost::javascript


#endif