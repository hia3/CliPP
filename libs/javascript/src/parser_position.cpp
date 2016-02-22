#include <strstream>
#include <boost/javascript/parser_position.hpp>

using namespace boost::javascript;

parser_position::parser_position (const char* first, const char* last, const std::string &file_name, int start_line)
:   start_line_(start_line)
,   code_(first, last)
,   current_(0)
,   file_name_(file_name)
{
}

parser_position::parser_position()
:   start_line_(1)
,   code_()
,   current_(0)
{
}

parser_position::parser_position(const parser_position& other)
:   start_line_(other.start_line_)
,   code_(other.code_)
,   current_(other.current_)
,   file_name_(other.file_name_)
{
}
parser_position& parser_position::operator=(const parser_position& other)
{
    if(this!=&other) {
        start_line_=other.start_line_;
        code_=other.code_;
        current_=other.current_;
        file_name_=other.file_name_;
    }
    return *this;
}

void parser_position::set_range(const char* first,const char* last)
{
    code_.assign(first, last);
}

void parser_position::set_file_name(const std::string& file_name)
{
    file_name_=file_name;
}

void parser_position::set_start_line(int start_line)
{
    start_line_=start_line;
}

bool parser_position::set_current(std::size_t current) //!< returns true if valid position, else returns false;
{
    if (current <= code_.size())
    {
        current_ = current;
        return true;
    }
    current_ = 0;
    return false;
}

int parser_position::get_current_line() const
{
    return get_line_at(current_);
}

int parser_position::get_line_at(std::size_t current) const
{
    std::string::const_iterator dummy;
    return get_line_at(current, dummy);
}

int parser_position::get_line_at(std::size_t current, std::string::const_iterator & line_start) const
{
    int line = start_line_;
    for (auto iter = code_.begin(), line_start = code_.begin(); iter != code_.end(); iter++)
    {
        if (*iter == '\n')
        {
            line++;
            line_start = iter + 1;
        }
        if (iter - code_.begin() == current)
        {
            return line;
        }
    }
    line_start = code_.end();
    return 0;
}

std::string parser_position::get_line_message(const std::string &pos_message) const
{
    std::string::const_iterator iter;
    int line = get_line_at(current_, iter);
    if (line == 0)
    {
        return "unknown position\n";
    }
    std::string message = "\n at codeline: ";
    std::strstream str;
    str << line << '\0';
    message += str.str();
    for (; iter != code_.end(); iter++)
    {
        if (*iter == '\n')
            break;
        if (iter - code_.begin() == current_)
        {
            message += pos_message;
        }
        message += *iter;
    }
    message += "\n";
    return message;
}
