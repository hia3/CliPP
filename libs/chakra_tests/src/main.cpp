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

#include <windows.h>

struct WScriptClass
{
    explicit WScriptClass(std::vector<std::string> & lines) : lines(lines)
    {
    }

    WScriptClass(WScriptClass const & other) : lines(other.lines)
    {

    }
    WScriptClass & operator = (WScriptClass const &) = delete;

    void Echo(std::string const & line)
    {
        lines.push_back(line);
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
        int success = 0, failure = 0, disabled = 0;

        auto exe = args.at(0);
        auto baselines_file_name = args.at(1);

        std::ifstream baselines_file(baselines_file_name);
        if (baselines_file)
        {
            do
            {
                std::string test_info;
                if (std::getline(baselines_file, test_info))
                {
                    std::string enabled, expected_result, baseline_file_name, script_file_name;

                    std::istringstream test_info_stream(test_info);
                    test_info_stream >> enabled >> expected_result >> baseline_file_name >> script_file_name;

                    if (enabled == "on")
                    {
                        int const result = system((exe + " " + baseline_file_name + " " + script_file_name).c_str());

                        bool const is_success = result == EXIT_SUCCESS;

                        std::string unexpected_result;
                        if (is_success && expected_result == "failure")
                        {
                            unexpected_result = "New test passed";
                        }

                        if (!is_success && expected_result == "success")
                        {
                            unexpected_result = "Regression detected";
                        }

                        if (unexpected_result.length() != 0)
                        {
                            ::MessageBoxA(0, (unexpected_result + "\n\n" + baseline_file_name + "\n" + script_file_name).c_str(), "clipp", MB_OK);
                        }

                        std::cout << "% " << (is_success ? "success" : "failure") << " " << baseline_file_name << " " << script_file_name << std::endl;

                        if (is_success)
                        {
                            success++;
                        }
                        else
                        {
                            failure++;
                        }
                    }
                    else
                    {
                        disabled++;
                    }
                }
            } while (baselines_file);
        }
        else
        {
            system("dir");
            std::cerr << "Can't open " << baselines_file_name << std::endl;
        }

        std::cout << "Total: " << "failed - " << failure << " succeeded - " << success << " disabled - " << disabled << std::endl;
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
            std::cerr << "baseline not found";
            return EXIT_FAILURE;
        }
        if (!script_file)
        {
            std::cerr << "script not found";
            return EXIT_FAILURE;
        }

        std::vector<std::string> echo;
        WScriptClass WScript(echo);
        {
            using namespace boost::clipp;
            using namespace boost::javascript;

            javascript_parser parser;
            auto context = parser.get_context();

            class_<WScriptClass/*, boost::clipp::reference_storage_policy*/> cls("WScriptClass", context);
            cls.function("Echo", &WScriptClass::Echo);

            context->global()->insert("WScript", wrap_ref(WScript, context));
            parser.parse(script_file);
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
                "HasMaybeEscapedUse",
                "DoStackNestedFunc",
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
                std::cerr << "Test failed - got `" << got << "` instead of " << baseline_line << ", line " << echo_idx << std::endl;
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
    DWORD s = ::GetTickCount();

    std::vector<std::string> args(argv, argv + argc);

    /*args.resize(3); // regression?
    args[1] = R"*(libs\tests\chakra\test\Basics\StringFromCharCode.baseline)*";
    args[2] = R"*(libs\tests\chakra\test\Basics\StringFromCharCode.js)*";*/

    int r = the_main(args);

    DWORD e = ::GetTickCount() - s;
    printf("\n\nTIME ::: %i\n\n", e);
    //::getchar();
    return r;
}
