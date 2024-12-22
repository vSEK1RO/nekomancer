#pragma once

#include <string>
#include <type_traits>
#include <nlohmann/json.hpp>
#include <nek/core/Exception.hpp>

namespace nek::core
{
    namespace Json
    {
        using Value = nlohmann::json;
    }
    /**
     * interface for json serializing features
     */
    struct IJsonable
    {
        virtual IJsonable &from(const Json::Value &json) = 0;
        virtual Json::Value toJson() const noexcept = 0;

        virtual IJsonable &parse(std::string_view sv);
        virtual std::string stringify() const noexcept;

        virtual ~IJsonable() = default;
    };

    template <typename T>
    concept IsIJsonable = requires(const Json::Value &json) {
        T{json};
    } && std::is_base_of_v<IJsonable, T>;

    template <IsIJsonable T, typename basic_json>
    void to_json(basic_json &j, const T &t)
    {
        j = t.toJson();
    }

    template <IsIJsonable T, typename basic_json>
    void from_json(const basic_json &j, T &t)
    {
        t.from(j);
    }

    namespace Json
    {
        template <typename T>
        concept IsConvertible = requires(Json::Value json, T t) {
            { json.get<T>() } -> std::same_as<T>;
            { Value{t} };
        };

        Value parse(std::string_view sv);
        std::string stringify(const Value &json) noexcept;

        template <typename T>
        Value from(const T &t)
        {
            if constexpr (IsConvertible<T>)
            {
                return Value(t);
            }
            else
            {
                throw Exception(Exception::JSON_FROM_TYPE);
            }
        }

        template <typename T>
        T to(const Value &json)
        {
            if constexpr (IsConvertible<T>)
            {
                return json.get<T>();
            }
            else
            {
                throw Exception(Exception::JSON_TO_TYPE);
            }
        }
    }
}