#include <test.hpp>
#undef ENTRY
#define ENTRY nek_core_Property

#include <memory>
#include <string>
#include <vector>

namespace ENTRY
{
    class CAlive : public IJsonable
    {
    public:
        Property<State<unsigned int>> health;
        Property<Computed<bool>> is_alive;

        CAlive(const Json::Value &json)
        {
            from(json);
        }

        CAlive &from(const Json::Value &json) override
        {
            health.emplace(json.at("health"));
            is_alive.emplace([this]()
                             { return health().get() != 0; });
            return *this;
        }
        Json::Value toJson() const noexcept override
        {
            Json::Value json;
            json["health"] = health().get();
            return json;
        }
    };

    TEST(ENTRY, move)
    {
        {
            Property<int> a;
            a.emplace(1);
            Property<int> b;
            b = std::move(a);
            EXPECT_ANY_THROW(a());
            EXPECT_NO_THROW(b());
        }
        {
            Property<int> a;
            bool notified = false;
            a.watch([&]()
                    { notified = true; });
            Property<int> b(std::move(a));
            a.emplace(1);
            EXPECT_FALSE(notified);
            b.emplace(1);
            EXPECT_TRUE(notified);
        }
    }

    TEST(ENTRY, constructor__emplace)
    {
        std::string json_string = R"({
            "health": {
                "default": 0,
                "value": 10
            }
        })";
        CAlive alive(Json::parse(json_string));

        EXPECT_EQ(alive.health().get(), 10);
        EXPECT_TRUE(alive.is_alive().get());

        alive.health().reset();
        EXPECT_FALSE(alive.is_alive().get());

        alive.health().set(10);
        EXPECT_TRUE(alive.is_alive().get());

        EXPECT_ANY_THROW(alive.health.emplace(1, 2));
    }

    TEST(ENTRY, operator_parentheses)
    {
        Property<int> a;
        EXPECT_ANY_THROW(a());

        EXPECT_NO_THROW(a.emplace(1));
        EXPECT_EQ(a(), 1);
        a() = 2;
        EXPECT_EQ(a(), 2);

        EXPECT_ANY_THROW(a.emplace(3));
    }

    TEST(ENTRY, get)
    {
        Property<int> a;
        EXPECT_FALSE(a.get());

        a.emplace(1);
        EXPECT_TRUE(a.get());
    }

    TEST(ENTRY, watch)
    {
        Property<bool> a;
        bool notified = 0;
        a.watch([&]()
                { notified = true; });
        Computed<bool> b([&]()
                         {
            try
            {
                return a();
            }
            catch(...)
            {
                return false;
            } });
        a.emplace(true);

        EXPECT_EQ(b.get(), true);
        EXPECT_TRUE(notified);
    }
}