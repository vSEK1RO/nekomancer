#pragma once

#include <string>
#include <json/json.h>

namespace nek::core
{
    struct IJsonify
    {
        virtual IJsonify &from(const Json::Value &json) = 0;
        virtual Json::Value toJson() const noexcept = 0;

        virtual IJsonify &parse(std::string_view sv);
        virtual std::string stringify() const noexcept;

        virtual ~IJsonify() = default;
    };
}