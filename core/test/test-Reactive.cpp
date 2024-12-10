#include <nek/core.hpp>

using namespace nek::core;

#undef ENTRY
#define ENTRY nek_core_Reactive

namespace ENTRY
{
    class A : public Reactive<int>
    {
    public:
        A() = default;
        int set(int v)
        {
            notify(this->v, v);
            this->v = v;
            return v;
        }

    private:
        int v = 0;
    };

    TEST(ENTRY, watch_unwatch)
    {
        A a;
        bool notified = false;
        auto &watcher = a.watch([&notified](auto, auto)
                                { notified = true; });
        a.set(1);
        EXPECT_TRUE(notified);
        EXPECT_ANY_THROW(a.watch(watcher));

        auto unwatched = a.unwatch(watcher);
        notified = false;
        a.set(0);
        EXPECT_FALSE(notified);
        EXPECT_NO_THROW(a.unwatch(unwatched));
    }
}