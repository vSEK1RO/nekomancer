#include <nek/core/Exception.hpp>

#include <sstream>

namespace nek::core
{
    const std::unordered_map<Exception::Code, std::string> Exception::messages = {
        {Code::JSON_PARSE, "exception during parsing json"},
        {Code::JSON_PROPERTY, "json doesn't have the following properties"},
        {Code::JSON_TO_TYPE, "json property cannot be converted to object"},
        {Code::JSON_FROM_TYPE, "object cannot be converted to json property"},
        {Code::ALREADY_WATCHED, "this Reactive is already watched by passed function / lambda"},
        {Code::NOT_WATCHED, "this Reactive isn't watched by passed function / lambda"},
        {Code::PROPERTY_INITIALIZED, "property constructor cannot be called after initialisation"},
        {Code::PROPERTY_UNINITIALIZED, "property is uninitialized"},
        {Code::COMPONENT_DYNAMIC_CAST, "passed component cannot be casted to its real type"},
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