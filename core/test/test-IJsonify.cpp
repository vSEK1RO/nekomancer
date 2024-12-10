#include <nek/core.hpp>

using namespace nek::core;

#undef ENTRY
#define ENTRY nek_core_IJsonify

namespace ENTRY
{
    struct A : public IJsonify
    {
        A() = default;
        A(int v) noexcept : v(v) {};
        A &from(const Json::Value &json) override
        {
            if (!json.isMember("v"))
            {
                throw Exception(Exception::PARSE_JSON, "doesn't have 'v' property");
            }
            v = json["v"].asInt();
            return *this;
        }
        Json::Value toJson() const noexcept override
        {
            Json::Value json;
            json["v"] = v;
            return json;
        }
        A &parse(std::string_view sv) override
        {
            return dynamic_cast<A &>(this->IJsonify::parse(sv));
        }
        ~A() = default;

        int v = 0;
    };

    TEST(ENTRY, parse)
    {
        std::string
            correct = R"({
                "v": 10
            })",
            incorrect1 = R"({
                wrong format
            })",
            incorrect2 = R"({
                "b": "string"
            })";
        EXPECT_EQ(ENTRY::A().parse(correct).v, 10);
        EXPECT_ANY_THROW(ENTRY::A().parse(incorrect1));
        EXPECT_ANY_THROW(ENTRY::A().parse(incorrect2));
    }

    TEST(ENTRY, stringify)
    {
        std::string correct = "{\n\t\"v\" : 10\n}";
        EXPECT_EQ(ENTRY::A(10).stringify(), correct);
    }
}