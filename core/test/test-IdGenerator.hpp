#include <test.hpp>
#undef ENTRY
#define ENTRY nek_core_IdGenerator

namespace ENTRY
{
    TEST(ENTRY, next_release)
    {
        IdGenerator<Component::Id> id_gen;
        EXPECT_EQ(id_gen.next(), 0);
        EXPECT_EQ(id_gen.next(), 1);
        id_gen.release(0);
        EXPECT_EQ(id_gen.next(), 0);
    }
}