#include <test.hpp>
#include <components/Picture.hpp>
#include <components/Alive.hpp>

#undef ENTRY
#define ENTRY nek_core_ComponentStore

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
    ComponentStore store(store_config, &manager);

    TEST(ENTRY, constructor)
    {
        auto &alive = store.get<nek::Alive>("CAlive");
        auto &picture = store.get<nek::Picture>("CPicture");
    }
}