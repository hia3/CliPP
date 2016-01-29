#include <boost/clipp.hpp>
#include <boost/javascript.hpp>


//#include <boost/clipp/clipp_stream.hpp>
//#include <boost/javascript/date.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <boost/config.hpp>
#include <boost/config/suffix.hpp>

struct WScriptClass
{
    /*WScriptClass()
    {

    }
    ~WScriptClass()
    {

    }
    WScriptClass(WScriptClass const &)
    {
        assert(false);
    }
    WScriptClass & operator = (WScriptClass const &)
    {
        assert(false);
    }*/

    void Echo(std::string const & line)
    {
        lines.push_back(line);
    }
    WScriptClass(std::vector<std::string> & lines) : lines(lines)
    {
    }

private:
    std::vector<std::string> & lines;
};

bool starts_with(std::string const & input, std::string const & pattern)
{
    bool result = 0 == input.compare(0, pattern.size(), pattern);
    return result;
}

int the_main(std::vector<std::string> args)
try
{
    switch (args.size())
    {
    case 2:
        {
            int success = 0, failure = 0;

            auto exe = args.at(0);
            auto baselines_file_name = args.at(1);

            std::ifstream baselines_file(baselines_file_name);
            if (baselines_file)
            {
                do
                {
                    std::string baseline_file_name;
                    if (std::getline(baselines_file, baseline_file_name))
                    {
                        std::cout << "baseline: " << baseline_file_name << " ";
                        auto script_file_name = baseline_file_name.substr(0, baseline_file_name.find_last_of(".")) + ".js";

                        int result = system((exe + " " + baseline_file_name + " " + script_file_name).c_str());

                        if (result == EXIT_SUCCESS)
                        {
                            success++;
                        }
                        else
                        {
                            failure++;
                        }
                        std::cout << failure << " " << success << std::endl;
                    }
                } while (baselines_file);
            }
            else
            {
                system("dir");
                std::cerr << "Can't open " << baselines_file_name << std::endl;
            }
        }
        break;
    case 3:
        {
            auto baseline_file_name = args.at(1);
            auto script_file_name = args.at(2);

            std::ifstream baseline_file(baseline_file_name);
            std::ifstream script_file(script_file_name);

            if (!baseline_file)
            {
                std::cout << "baseline not found";
            }
            if (!script_file)
            {
                std::cout << "script not found";
            }
            std::cout << std::endl;

            std::vector<std::string> echo;
            {
                using namespace boost::clipp;
                using namespace boost::javascript;

                javascript_parser parser;
                auto context = parser.get_context();

                class_<WScriptClass> cls("WScriptClass", context);
                cls.function("Echo", &WScriptClass::Echo);

                context->global()->insert("WScript", wrap(new WScriptClass(echo), context));
                parser.parse(script_file);
                //context->global()->erase(wrapped_WScript);
            }

            std::size_t echo_idx = 0;
            do
            {
                std::string baseline_line;
                if (!std::getline(baseline_file, baseline_line))
                {
                    break;
                }
                std::vector<std::string> ignore_lines
                {
                    "HasFuncDecl",
                    "HasFuncAssignment",
                    "HasMaybeEscapedNestedFunc",
                    "TODO"
                };
                bool ignore = std::any_of(ignore_lines.begin(), ignore_lines.end(), [&](auto & ignore_line) { return starts_with(baseline_line, ignore_line); });
                if (ignore)
                {
                    continue;
                }
                std::string got = echo_idx < echo.size() ? echo.at(echo_idx) : "***empty***";
                if (got == baseline_line)
                {
                    echo_idx++;
                }
                else
                {
                    std::cerr << "Test failed - got `" << got << "` instead of " << baseline_line << std::endl;
                    return EXIT_FAILURE;
                }

            } while (baseline_file);
        }
        break;
    }

    return EXIT_SUCCESS;
}
catch (std::exception const & e)
{
    std::cerr << "Fatal error: " << e.what() << std::endl;
    return EXIT_FAILURE;
}
catch (...)
{
    std::cerr << "Fatal error." << std::endl;
    return EXIT_FAILURE;
}

int main(int argc, char * argv[])
{
    std::vector<std::string> args(argv, argv + argc);
    /*args.resize(3);
    args[1] = R"*(libs\tests\chakra\test\strict\09.ObjectLiteral.baseline)*";
    args[2] = R"*(libs\tests\chakra\test\strict\09.ObjectLiteral.js)*";*/
    return the_main(args);
}
