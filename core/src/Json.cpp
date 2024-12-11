#include <nek/core/Json.hpp>

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
                throw Exception(Exception::PARSE_JSON, e.what());
            }
        }
        std::string stringify(const Value &json) noexcept
        {
            return json.dump(4);
        }
    }
}