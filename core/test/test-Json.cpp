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

        bool operator==(const A &rhs) const noexcept
        {
            return v == rhs.v;
        }

        A &from(const Json::Value &json) override
        {
            if (json.contains("v"))
            {
                v = Json::to<int>(json["v"]);
            }
            else
            {
                throw Exception(Exception::JSON_PROPERTY, "v");
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
        EXPECT_EQ(A(Json::parse(A(10).stringify())).v, 10);
    }

    struct B
    {
    };

    TEST(ENTRY, concepts)
    {
        EXPECT_TRUE(IsIJsonable<A>);
        EXPECT_FALSE(IsIJsonable<B>);
    }

    TEST(ENTRY, basic_types)
    {
        Json::Value json = -1;
        EXPECT_EQ(Json::to<int>(json), -1);
        EXPECT_EQ(Json::to<uint32_t>(json), -1u);
    }

    TEST(ENTRY, custom_serializer)
    {
        std::vector<A> a = {1, 2};
        auto json = Json::from(a);
        auto b = Json::to<std::vector<A>>(json);
        EXPECT_EQ(a, b);
    }
}