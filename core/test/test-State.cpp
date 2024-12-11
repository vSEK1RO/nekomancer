#include <test.hpp>
#undef ENTRY
#define ENTRY nek_core_State

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

    auto unwatched = a.unwatch(watcher);
    a.set(1);
    EXPECT_FALSE(notified);
    EXPECT_NO_THROW(a.unwatch(unwatched));
}