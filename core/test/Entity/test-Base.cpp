#include <test.hpp>
#undef ENTRY
#define ENTRY nek_core_Entity

#include <components/Alive.hpp>

namespace ENTRY
{
    Json::Value manager_config = {
        {"CAlive", "./components/CAlive.so"},
        {"CPicture", "./components/CPicture.so"}};

    Json::Value alive_config = {
        {"health", {{"default", 0}, {"value", 10}}},
        {"mana", {{"default", 10}}}};

    Json::Value store_config = {
        {"CAlive", alive_config},
        {"CPicture", {}}};

    ComponentManager manager(manager_config);

    TEST(ENTRY, constructor_from_toJson)
    {
        Entity person;
        person.store.emplace(&manager);
        person.from(store_config);
        auto &alive = person.store().get<nek::Alive>("CAlive");
        EXPECT_EQ(alive.health().get(), 10);
    }
}