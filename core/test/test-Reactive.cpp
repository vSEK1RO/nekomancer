#include <test.hpp>
#undef ENTRY
#define ENTRY nek_core_Reactive

namespace ENTRY
{
    class A : public Reactive
    {
    public:
        A() = default;
        int set(int v)
        {
            Reactive::notify();
            this->v = v;
            return v;
        }

    private:
        int v = 0;
    };

    TEST(ENTRY, move)
    {
        Reactive a;
        auto &watcher = a.watch([](){});
        Reactive b;
        b = std::move(a);
        EXPECT_ANY_THROW(a.getWatcher(watcher));
        EXPECT_NO_THROW(b.getWatcher(watcher));
    }

    TEST(ENTRY, watch_unwatch)
    {
        A a;
        bool notified = false;
        auto &watcher = a.watch([&notified]()
                                { notified = true; });
        a.set(1);
        EXPECT_TRUE(notified);
        EXPECT_ANY_THROW(a.watch(watcher));
        EXPECT_ANY_THROW(a.watch(std::move(watcher)));

        auto unwatched = a.getWatcher(watcher);
        a.unwatch(watcher);
        notified = false;
        a.set(0);
        EXPECT_FALSE(notified);
        EXPECT_ANY_THROW(a.getWatcher(unwatched));
    }

    struct B : public Reactive
    {
        void trackEnable() const
        {
            Reactive::do_track = true;
        }
        void watchTracked(bool &notified) const
        {
            Reactive::watchTracked([&]()
                                   { notified = true; });
        }
    };

    TEST(ENTRY, trackSelf_watchTracked)
    {
        B b;
        State<int> a = 0;
        bool notified = false;

        b.trackEnable();
        a.get();
        b.watchTracked(notified);
        a.set(1);
        EXPECT_TRUE(notified);
    }
}