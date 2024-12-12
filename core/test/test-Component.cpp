#include <test.hpp>
#undef ENTRY
#define ENTRY nek_core_Component

#include <memory>
#include <string>
#include <vector>

namespace ENTRY
{
    class CAlive : public IJsonable
    {
    private:
        std::unique_ptr<State<unsigned int>> _health;
        std::unique_ptr<Computed<bool>> _is_alive;

    public:
        State<unsigned int> &health() const
        {
            if (_health.get() == nullptr)
            {
                throw Exception(Exception::PROPERTY_UNINITIALIZED);
            }
            return *_health;
        };
        Computed<bool> &is_alive() const
        {
            if (_is_alive.get() == nullptr)
            {
                throw Exception(Exception::PROPERTY_UNINITIALIZED);
            }
            return *_is_alive;
        };

        CAlive(const Json::Value &json)
        {
            from(json);
        }

        CAlive &from(const Json::Value &json) override
        {
            _health = std::make_unique<State<unsigned int>>(json.at("health"));
            _is_alive = std::make_unique<Computed<bool>>([this]()
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

    // class CPicture : public IJsonify
    // {
    // public:
    //     std::unique_ptr<Computed<std::string>> path;
    // }

    TEST(ENTRY, CAlive_CPicture)
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
    }
}