#include <test.hpp>
#undef ENTRY
#define ENTRY nek_core_State

namespace ENTRY
{
    TEST(ENTRY, constructor_get_set_reset)
    {
        {
            State<int> s = 0;
            EXPECT_EQ(s.get(), 0);
            EXPECT_EQ(s.set(1).get(), 1);
            EXPECT_EQ(s.reset().get(), 0);
        }
        {
            State<int> s{1, 2};
            EXPECT_EQ(s.get(), 2);
            EXPECT_EQ(s.reset().get(), 1);
        }
    }

    TEST(ENTRY, watch_unwatch)
    {
        State<int> a = 0;
        bool notified = false;
        int _old_value = 0, _value = 0;
        a.set(2);
        auto &watcher = a.watch([&](int old_value, int value)
                                {
            notified = true;
            _old_value = old_value;
            _value = value; });
        a.set(1);
        EXPECT_TRUE(notified && _old_value == 2 && _value == 1);
        EXPECT_ANY_THROW(a.watch(watcher));

        notified = false;
        a.reset();
        EXPECT_TRUE(notified && _old_value == 1 && _value == 0);

        notified = false;
        a.reset();
        EXPECT_FALSE(notified);

        auto unwatched = a.getWatcher(watcher);
        a.unwatch(watcher);
        a.set(1);
        EXPECT_FALSE(notified);
        EXPECT_ANY_THROW(a.getWatcher(unwatched));
    }

    TEST(ENTRY, IJsonable)
    {
        {
            State<int> a(Json::parse(R"({
                "default": 1,
                "value": 2
            })"));
            EXPECT_EQ(a.get(), 2);
            EXPECT_EQ(a.reset().get(), 1);
        }
        {
            struct A : IJsonable
            {
                std::vector<int> v;

                A() = default;
                A(const Json::Value &json)
                {
                    from(json);
                }
                bool operator==(const A &rhs) const noexcept
                {
                    if (v.size() != rhs.v.size())
                    {
                        return false;
                    }
                    for (size_t i = 0; i < v.size(); i++)
                    {
                        if (v[i] != rhs.v[i])
                        {
                            return false;
                        }
                    }
                    return true;
                }
                A &from(const Json::Value &json)
                {
                    v = Json::to<std::vector<int>>(json.at("v"));
                    return *this;
                }
                Json::Value toJson() const noexcept
                {
                    Json::Value json;
                    json["v"] = Json::from<std::vector<int>>(v);
                    return json;
                }
            };
            State<A> a(Json::parse(R"({
                "default": {
                    "v": [1]
                },
                "value": {
                    "v": []
                }
            })"));
            EXPECT_EQ(a.get().v.size(), 0);
            EXPECT_EQ(a.reset().get().v[0], 1);
        }
    }

    enum Status : uint8_t
    {
        CREATED,
        DELETED,
    };
    NLOHMANN_JSON_SERIALIZE_ENUM(Status, {
                                             {Status::CREATED, "CREATED"},
                                             {Status::DELETED, "DELETED"},
                                         })

    TEST(ENTRY, non_IJsonable)
    {
        {
            enum AnotherStatus : uint8_t
            {
                CREATED,
                DELETED,
            };
            State<AnotherStatus> a(AnotherStatus::CREATED);
            EXPECT_ANY_THROW(a.from(Json::Value()));
        }
        {
            State<Status> a(Status::CREATED);
            Json::Value json({
                {"default", "CREATED"},
                {"value", "DELETED"},
            });
            EXPECT_NO_THROW(a.from(json));
            EXPECT_EQ(a.toJson(), json);
        }
    }
}