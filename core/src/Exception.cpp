#include <nek/core/Exception.hpp>

#include <sstream>

namespace nek::core
{
    const std::unordered_map<Exception::Code, std::string> Exception::messages = {
        {Code::PARSE_JSON, "exception during parsing json"},
        {Code::PROPERTY_JSON, "json doesn't have the following properties"},
        {Code::PROPERTY_TYPE_JSON, "json property cannot be converted to object"},
        {Code::OBJECT_TYPE_JSON, "object cannot be converted to json property"},
        {Code::ALREADY_WATCHED, "this Reactive already watched by passed function / lambda"},
    };
    Exception::Exception(Code code, std::string_view description, const std::source_location &location)
    {
        _code = code;
        std::ostringstream oss;
        oss << "\033[34m"
            << location.function_name()
            << ':'
            << location.line()
            << ": \033[31mnek::core::Exception:\t\033[0m"
            << message()
            << "\n\n";
        if (!description.empty())
        {
            oss << description << "\n";
        }
        _what = oss.str();
    }
}