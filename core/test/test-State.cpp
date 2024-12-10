#include <test.hpp>
#undef ENTRY
#define ENTRY nek_core_State

TEST(ENTRY, constructor_get_set_reset)
{
    State<int> a = 0;
    EXPECT_EQ(a.get(), 0);
    EXPECT_EQ(a.set(1).get(), 1);
    EXPECT_EQ(a.reset().get(), 0);
    a = State<int>{1, 2};
    EXPECT_EQ(a.get(), 2);
    EXPECT_EQ(a.reset().get(), 1);
}
TEST(ENTRY, watch_unwatch)
{
    State<int> a = 0;
    bool notified = false;
    int _old_value = 0, _value = 0;
    auto &watcher = a.watch([&](int old_value, int value)
                            {
        notified = true;
        _old_value = old_value;
        _value = value; });
    a.set(1);
    EXPECT_TRUE(notified && _old_value == 0 && _value == 1);
    EXPECT_ANY_THROW(a.watch(watcher));

    notified = false;
    a.reset();
    EXPECT_TRUE(notified && _old_value == 1 && _value == 0);

    auto unwatched = a.unwatch(watcher);
    notified = false;
    a.set(1);
    EXPECT_FALSE(notified);
    EXPECT_NO_THROW(a.unwatch(unwatched));
}