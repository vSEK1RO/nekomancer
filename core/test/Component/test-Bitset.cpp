#include <test.hpp>
#undef ENTRY
#define ENTRY nek_core_ComponentBitset

namespace ENTRY
{
    Json::Value manager_config = {
        {"CAlive", "./components/CAlive.so"},
        {"CPicture", "./components/CPicture.so"}};

    ComponentManager manager(manager_config);

    TEST(ENTRY, from__operator_eq__has)
    {
        ComponentBitset b1, b2;
        b1.manager.emplace(&manager);
        b2.manager.emplace(&manager);

        EXPECT_NO_THROW(b2.fromNames({"CAlive", "CPicture"}));
        EXPECT_NO_THROW(b1.fromNames({"CAlive"}));
        EXPECT_FALSE(b1 == b2);
        EXPECT_FALSE(b1.has("CPicture"));
        EXPECT_TRUE(b2.has(b1));
        EXPECT_TRUE(b1.has(b1));
        EXPECT_FALSE(b1.has(b2));
    }
}