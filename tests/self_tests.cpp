#include "NTest.h"

TEST(TestRegistrationWorks) {
    ASSERT_TRUE(NTest::get_registered_tests().size() >= 1);
}

TEST(AssertEqFails) {
    bool threw = false;
    try {
        ASSERT_EQ(1, 2);
    } catch (...) {
        threw = true;
    }
    ASSERT_TRUE(threw);
}

TEST(AssertEgNoFails){
    bool threw = false;
    try {
        ASSERT_EQ(1,1);
    } catch(...) {
        threw = true;
    }
    ASSERT_FALSE(threw);
}

static std::vector<int> hits;

void ParamTestWrapper(int x){
    hits.push_back(x);
}

TEST(ParamTestRegistration){
    hits.clear();
    for(int val : {1, 2, 3}){
        NTest::get_registered_tests().push_back(new NTest::TestCase(
            ("param_test_" + std::to_string(val)).c_str(),
            [val] { ParamTestWrapper(val); }
        ));
    }
}

int main() {
    return NTest::run_all();

    ASSERT_EQ(hits.size(), 3);
    ASSERT_EQ(hits[0], 1);
    ASSERT_EQ(hits[1], 2);
    ASSERT_EQ(hits[2], 3);
}
