#pragma once

#include <cstdint>
#include <exception>
#include <source_location>
#include <string>
#include <unordered_map>

namespace nek::core
{
    class Exception : public std::exception
    {
    public:
        enum Code
        {
            PARSE_JSON,
            PROPERTY_JSON,
            PROPERTY_TYPE_JSON,
            OBJECT_TYPE_JSON,
            ALREADY_WATCHED,
            PROPERTY_UNINITIALIZED,
        };
        static const std::unordered_map<Code, std::string> messages;

        Exception(Code code, std::string_view description = "", const std::source_location &location = std::source_location::current());
        const char *what() const noexcept override
        {
            return _what.c_str();
        }
        const char *message() const noexcept
        {
            return messages.at(_code).c_str();
        }
        Code code() const noexcept
        {
            return _code;
        }

    private:
        Code _code;
        std::string _what;
    };
}