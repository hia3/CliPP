#include <strstream>
#include <boost/javascript/parser_position.hpp>

using namespace boost::javascript;

parser_position::parser_position (const char* first, const char* last, const std::string &file_name, int start_line)
:   start_line_(start_line)
,   first_(first)
,   last_(last)
,   current_(first)
,   file_name_(file_name)
{
}

parser_position::parser_position()
:   start_line_(1)
,   first_(0)
,   last_(0)
,   current_(0)
{
}

parser_position::parser_position(const parser_position& other)
:   start_line_(other.start_line_)
,   first_(other.first_)
,   last_(other.last_)
,   current_(other.current_)
,   file_name_(other.file_name_)
{
}
parser_position& parser_position::operator=(const parser_position& other)
{
    if(this!=&other) {
        start_line_=other.start_line_;
        first_=other.first_;
        last_=other.last_;
        current_=other.current_;
        file_name_=other.file_name_;
    }
    return *this;
}

void parser_position::set_range(const char* first,const char* last)
{
    first_=first;
    last_=last;
}

void parser_position::set_file_name(const std::string& file_name)
{
    file_name_=file_name;
}

void parser_position::set_start_line(int start_line)
{
    start_line_=start_line;
}

bool parser_position::set_current(const char* current) //!< returns true if valid position, else returns false;
{
    if ((current >= first_) && (current <= last_))
    {
        current_ = current;
        return true;
    }
    current_ = NULL;
    return false;
}

int parser_position::get_current_line() const
{
    const char *dummy;
    return get_line_at(current_, dummy);
}

int parser_position::get_line_at(const char *current) const
{
    const char *dummy;
    return get_line_at(current, dummy);
}

int parser_position::get_line_at(const char *current, const char *&line_start) const
{
    if (current == NULL)
    {
        line_start = NULL;
        return 0;
    }
    const char *iter;
    int line = start_line_;
    for (iter = first_, line_start = first_; iter != last_; iter++)
    {
        if (*iter == '\n')
        {
            line++;
            line_start = iter + 1;
        }
        if (iter == current)
        {
            return line;
        }
    }
    line_start = NULL;
    return 0;
}

std::string parser_position::get_line_message(const std::string &pos_message) const
{
    std::string message = "\n at codeline: ";
    const char *iter;
    int line = get_line_at(current_, iter);
    if (line == 0)
    {
        message = "unknown position\n";
        return message;
    }
    std::strstream str;
    str << line << '\0';
    message += str.str();
    for (iter ; iter != last_; iter++)
    {
        if (*iter == '\n')
            break;
        if (iter == current_)
        {
            message += pos_message;
        }
        message += *iter;
    }
    message += "\n";
    return message;
}
