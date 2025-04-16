#include "NTest.h"

TEST(AdditionWorks) {
    ASSERT_EQ(2+2, 4);
}

TEST(TruthIsTrue)
{
    ASSERT_TRUE(true);
}

int main() {
    return NTest::run_all();
}
