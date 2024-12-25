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

    TEST(ENTRY, constructor)
    {
        IObserver &plog = ObserverPlog::getInstance();

        ComponentManager manager;
        manager.message().watch(plog.getWatcher());
        manager.from(manager_config);

        ComponentStore store;
        store.manager.emplace(&manager);
        store.addObserver(plog);
        store.from(store_config);

        auto &alive = store.get<nek::Alive>("CAlive");
        auto &picture = store.get<nek::Picture>("CPicture");

        EXPECT_EQ(alive.health().get(), 10);
        EXPECT_EQ(picture.path().get(), "alive.png");

        alive.health().reset();
        EXPECT_EQ(alive.health().get(), 0);
        EXPECT_EQ(picture.path().get(), "dead.png");
    }
}