#include <test.hpp>
#undef ENTRY
#define ENTRY nek_core_Json

namespace ENTRY
{
    struct A : public IJsonable
    {
        A() = default;
        A(int v) noexcept : v(v) {};
        A(const Json::Value &json)
        {
            from(json);
        }

        A &from(const Json::Value &json) override
        {
            if (json.contains("v"))
            {
                v = Json::to<int>(json["v"]);
            }
            else
            {
                throw Exception(Exception::PROPERTY_JSON, "v");
            }
            return *this;
        }
        Json::Value toJson() const noexcept override
        {
            Json::Value json;
            json["v"] = Json::from<int>(v);
            return json;
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
        EXPECT_EQ(ENTRY::A(Json::parse(correct)).v, 10);
        EXPECT_ANY_THROW(ENTRY::A(Json::parse(incorrect1)));
        EXPECT_ANY_THROW(ENTRY::A(Json::parse(incorrect2)));
    }

    TEST(ENTRY, stringify)
    {
        std::string correct = "{\n    \"v\": 10\n}";
        EXPECT_EQ(ENTRY::A(10).stringify(), correct);
    }

    struct B
    {
    };

    TEST(ENTRY, concepts)
    {
        EXPECT_TRUE(IsIJsonable<A>);
        EXPECT_FALSE(IsIJsonable<B>);
        EXPECT_TRUE(Json::IsConvertible<int>);
        EXPECT_FALSE(Json::IsConvertible<Reactive>);
    }
}