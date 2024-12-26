#include <test.hpp>
#undef ENTRY
#define ENTRY nek_core_EntityStore

#include <components/Alive.hpp>

namespace ENTRY
{
    Json::Value manager_config = {
        {"CAlive", "./components/CAlive.so"},
        {"CPicture", "./components/CPicture.so"}};

    Json::Value alive_config = {
        {"health", {{"default", 0}, {"value", 10}}},
        {"mana", {{"default", 10}}}};

    Json::Value entity_2 = {
        {"CAlive", alive_config},
        {"CPicture", {}}};
    
    Json::Value entity_1 = {
        {"CAlive", alive_config}};
    

    ComponentManager manager(manager_config);
    auto &plog = ObserverPlog::getInstance();

    TEST(ENTRY, create_remove)
    {
        EntityStore store;
        store.manager.emplace(&manager);
        store.addObserver(plog);
        auto a = store.create(entity_2);
        auto b = store.create(entity_2);
        EXPECT_EQ(b->id(), 1);
        EXPECT_ANY_THROW(store.remove(10));
        EXPECT_NO_THROW(store.remove(a->id()));
        auto c = store.create(entity_1);
        EXPECT_EQ(c->id(), 0);
    }

    TEST(ENTRY, get)
    {
        EntityStore store;
        store.manager.emplace(&manager);
        store.addObserver(plog);
        auto a = store.create(entity_2);
        auto b = store.create(entity_1);
        EXPECT_EQ(store.get({"CAlive", "CPicture"}).size(), 1);
        EXPECT_EQ(store.get({"CAlive"}).size(), 2);
    }
}