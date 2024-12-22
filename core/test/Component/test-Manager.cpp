#include <test.hpp>
#include <components/Alive.hpp>

#undef ENTRY
#define ENTRY nek_core_ComponentManager

namespace ENTRY
{
    Json::Value config = {{"CAlive", "./components/CAlive.so"}};
    Json::Value component_config = {
        {"health", 10},
        {"mana", 10}};

    ComponentManager manager(config);

    std::shared_ptr<IComponent> ptr;
    std::shared_ptr<nek::Alive> casted_ptr;

    TEST(ENTRY, constructor_from_id_name_has)
    {
        EXPECT_TRUE(manager.has("CAlive"));
        EXPECT_FALSE(manager.has("123"));
        EXPECT_EQ(manager.id("CAlive"), 0);
        EXPECT_EQ(manager.name(0), "CAlive");
    }

    TEST(ENTRY, construct)
    {
        EXPECT_NO_THROW(ptr = manager.create("CAlive"));
        EXPECT_EQ(ptr->id(), 0);
        EXPECT_NO_THROW(casted_ptr = manager.create<nek::Alive>("CAlive"));
        EXPECT_EQ(casted_ptr->id(), 0);

        casted_ptr->from(component_config);
        EXPECT_EQ(casted_ptr->health(), 10);
        EXPECT_EQ(casted_ptr->mana(), 10);
    }

    TEST(ENTRY, mount_unmount)
    {
        EXPECT_EQ(casted_ptr->status().get(), Component::Status::CREATED);
        casted_ptr->mount();
        EXPECT_EQ(casted_ptr->status().get(), Component::Status::MOUNTED);
        casted_ptr->unmount();
        EXPECT_EQ(casted_ptr->status().get(), Component::Status::UNMOUNTED);
    }
}