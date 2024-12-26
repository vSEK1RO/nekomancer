#include <test.hpp>
#undef ENTRY
#define ENTRY nek_core_Computed

TEST(ENTRY, constructor_watch_unwatch)
{
    State<uint32_t> a = 2;
    State<int> b = 2;
    Computed<uint64_t> c([&]()
                         { return a.get() * b.get(); });
    EXPECT_EQ(c.get(), 4);
    a.set(4);
    EXPECT_EQ(c.get(), 8);
    EXPECT_ANY_THROW(Computed<int>([&]()
                                   { throw std::logic_error("incomputed"); return 1; }));

    bool notified = false;
    uint64_t _old_value = 0, _value = 0;
    auto &watcher = c.watch([&](uint64_t old_value, uint64_t value)
                            {
        notified = true;
        _old_value = old_value;
        _value = value; });
    a.set(3);
    EXPECT_TRUE(notified);
    EXPECT_EQ(_old_value, 8);
    EXPECT_EQ(_value, 6);
    a.set(4);
    EXPECT_EQ(_old_value, 6);
    EXPECT_EQ(_value, 8);

    EXPECT_NO_THROW(c.getWatcher(watcher));
    c.unwatch(watcher);

    notified = false;
    a.set(2);
    EXPECT_FALSE(notified);
}

TEST(ENTRY, Reactive_watch)
{
    State<uint32_t> a = 2;
    State<int> b = 2;
    Computed<uint64_t> c([&]()
                         { return a.get() * b.get(); });

    bool notified = false;
    c.Reactive::watch([&notified]()
                      { notified = true; });
    a.set(4);
    EXPECT_TRUE(notified);
}

TEST(ENTRY, destructor)
{
    State<int> s = 2;
    {
        Computed<int> c([&]()
                        { return s.get(); });
    }
    EXPECT_NO_THROW(s.set(1));
}