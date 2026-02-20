#include <cassert>
#include <string>
#include <vector>

#include <NTest.h>

// ======================================================
// Registration basics
// ======================================================

TEST(TestRegistrationWorks) {
    REQUIRE(NTest::get_registered_tests().size() >= 1);
    ASSERT_TRUE(NTest::get_registered_tests().size() >= 1);
}

// ======================================================
// REQUIRE / ASSERT behavior
// ======================================================

TEST(AssertEqFails) {
    bool threw = false;
    try {
        REQUIRE_EQ(1, 2);
    } catch (...) {
        threw = true;
    }
    REQUIRE(threw);
}

TEST(AssertEqNoFails) {
    bool threw = false;
    try {
        REQUIRE_EQ(1, 1);
    } catch (...) {
        threw = true;
    }
    REQUIRE(!threw);
}

TEST(AssertAbortsTest) {
    bool threw = false;
    try {
        ASSERT_TRUE(false);
    } catch (...) {
        threw = true;
    }
    ASSERT_TRUE(threw);
}

// ======================================================
// EXPECT behavior (non-fatal)
// ======================================================

TEST(ExpectDoesNotAbort) {
    EXPECT(false);
    ASSERT_TRUE(true);  // must still run
}

// ======================================================
// REQUIRE stops execution (verified indirectly)
// ======================================================

static int require_hit_count = 0;

TEST_EXPECT_FAIL(RequireStopsExecution) {
    require_hit_count++;
    REQUIRE(false);
    require_hit_count++;  // must NOT run
}

TEST(RequireDidStopExecution) { ASSERT_EQ(require_hit_count, 1); }

// ======================================================
// REQUIRE inside helper function
// ======================================================

static int helper_hit_count = 0;

void helper_require_fail() {
    helper_hit_count++;
    REQUIRE(false);
    helper_hit_count++;  // must NOT run
}

TEST_EXPECT_FAIL(RequireInsideHelper) { helper_require_fail(); }

TEST(RequireInsideHelperDidAbort) { ASSERT_EQ(helper_hit_count, 1); }

// ======================================================
// Suite continuation after failures
// ======================================================

static int suite_progress = 0;

TEST_EXPECT_FAIL(SuiteFailureOccurs) {
    suite_progress++;
    ASSERT_TRUE(false);  // test must fail
}

TEST(SuiteContinuesAfterFailure) {
    suite_progress++;
    ASSERT_EQ(suite_progress, 2);
}

// ======================================================
// Exception handling
// ======================================================

static int exception_hits = 0;

TEST_EXPECT_FAIL(UnhandledStdExceptionIsCaught) {
    exception_hits++;
    throw std::runtime_error("boom");
}

TEST_EXPECT_FAIL(NonStdExceptionIsCaught) {
    exception_hits++;
    throw 42;
}

TEST(ExceptionTestsRan) { ASSERT_EQ(exception_hits, 2); }

// ======================================================
// Registration order
// ======================================================

static std::vector<int> order;

TEST(Order1) { order.push_back(1); }
TEST(Order2) { order.push_back(2); }
TEST(Order3) { order.push_back(3); }

TEST(RegistrationOrderPreserved) {
    ASSERT_EQ(order.size(), 3);
    ASSERT_EQ(order[0], 1);
    ASSERT_EQ(order[1], 2);
    ASSERT_EQ(order[2], 3);
}

// ======================================================
// Entry point
// ======================================================

int main() {
    return NTest::run_all();
    // int failed = NTest::run_all();
    // std::cout << "Num Failed: " << failed
    //           << "; Expected: " << NTest::EXPECTED_FAILURES() << "\n";
    // return failed == NTest::EXPECTED_FAILURES() ? 0 : failed;
}
