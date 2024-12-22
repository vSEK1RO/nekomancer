#include <test.hpp>
#include <components/Alive.hpp>

#undef ENTRY
#define ENTRY nek_core_ComponentManager

namespace ENTRY
{
    TEST(ENTRY, constructor_from_id_has_construct)
    {
        Json::Value config = {{"CAlive", "./components/CAlive.so"}};
        Json::Value component_config = {
            {"health", 10},
            {"mana", 10}
        };

        ComponentManager manager(config);
        EXPECT_TRUE(manager.has("CAlive"));
        EXPECT_FALSE(manager.has("123"));
        EXPECT_EQ(manager.id("CAlive"), 0);

        std::shared_ptr<IComponent> ptr;
        std::shared_ptr<nek::Alive> casted_ptr;
        EXPECT_NO_THROW(ptr = manager.create("CAlive"));
        EXPECT_EQ(ptr->id(), 0);
        EXPECT_NO_THROW(casted_ptr = manager.create<nek::Alive>("CAlive"));
        EXPECT_EQ(casted_ptr->id(), 0);

        casted_ptr->from(component_config);
        EXPECT_EQ(casted_ptr->health(), 10);
        EXPECT_EQ(casted_ptr->mana(), 10);
    }
}