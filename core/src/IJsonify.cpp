#include <nek/core/IJsonify.hpp>

#include <sstream>
#include <nek/core/Exception.hpp>

namespace nek::core
{
    IJsonify &IJsonify::parse(std::string_view sv)
    {
        Json::CharReaderBuilder reader;
        Json::Value json;
        std::istringstream iss{std::string(sv)};
        std::string err;

        if (!Json::parseFromStream(reader, iss, &json, &err))
        {
            throw Exception(Exception::PARSE_JSON, err);
        }
        return from(json);
    }
    std::string IJsonify::stringify() const noexcept
    {
        Json::StreamWriterBuilder writer;
        return Json::writeString(writer, toJson());
    }
}