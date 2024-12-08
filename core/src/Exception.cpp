#include <nek/core/Exception.hpp>

#include <sstream>

namespace nek::core
{
    const std::unordered_map<Exception::Code, std::string> Exception::messages = {
        {Code::PARSE_JSON, "exception during parsing json"},
    };
    Exception::Exception(Code code, std::string_view description, const std::source_location &location)
    {
        _code = code;
        std::ostringstream oss;
        oss << "\033[34m"
            << location.function_name()
            << ':'
            << location.line()
            << ": \033[31mnek::core::Exception:\n\t\033[0m"
            << message()
            << "\n";
        if (!description.empty())
        {
            oss << description << "\n";
        }
        _what = oss.str();
    }
}