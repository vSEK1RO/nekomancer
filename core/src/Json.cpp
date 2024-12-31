#include <json-schema.hpp>
#include <nek/core/Json.hpp>
#include <nek/core/Exception.hpp>

namespace nek::core
{
    IJsonable &IJsonable::parse(std::string_view sv)
    {
        return from(Json::parse(sv));
    }

    std::string IJsonable::stringify() const noexcept
    {
        return Json::stringify(toJson());
    }

    IJsonable &IJsonable::from(const Json::Value &)
    {
        return *this;
    }

    Json::Value IJsonable::toJson() const noexcept
    {
        return Json::Value();
    }

    namespace Json
    {
        Value parse(std::string_view sv)
        {
            try
            {
                return nlohmann::json::parse(sv);
            }
            catch (const Value::parse_error &e)
            {
                throw Exception(Exception::JSON_PARSE, e.what());
            }
        }

        std::string stringify(const Value &json) noexcept
        {
            return json.dump(2);
        }

        void validate(const Value &json, const Value &schema)
        {
            nlohmann::json_schema::json_validator validator;
            validator.set_root_schema(schema);
            try
            {
                validator.validate(json);
            }
            catch (const std::exception &e)
            {
                throw Exception(Exception::JSON_VALIDATION, Json::to<std::string>(schema.at("name")) + ": " + e.what());
            }
        }
    }
}