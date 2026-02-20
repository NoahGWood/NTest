#include <NTest.h>

TEST(AdditionWorks) {
    REQUIRE_EQ(2 + 2, 4);
    ASSERT_EQ(2 + 2, 4);
}

TEST(TruthIsTrue) {
    REQUIRE(true);
    ASSERT_TRUE(true);
}

int main() { return NTest::run_all(); }
