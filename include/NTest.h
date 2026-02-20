/**
 * @file NTest.h
 * @author NoahGWood
 * @brief Single-header unit test framework (simple, fatal + non-fatal
 * assertions)
 * @version 0.2
 * @date 2025-04-15
 */
#ifndef NTEST_UNIT_H
#define NTEST_UNIT_H

#define NTEST_VERSION "0.2"

#include <AnsiColor.h>

#include <functional>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#define NTEST_COLOR(x) NTest::Color::x

namespace NTest {

    // =========================
    // Test registration
    // =========================

    struct TestCase {
        const char* name;
        std::function<void()> func;
        bool expect_fail = false;

        TestCase(const char* /* name */, std::function<void()> /* func */);
    };

    // Function-local static ensures one registry per program
    inline std::vector<TestCase*>& REGISTRY() {
        static std::vector<TestCase*> tests;
        return tests;
    }

    // =========================
    // Backwards Compatibility
    // =======================
    inline std::vector<TestCase*>& get_registered_tests() { return REGISTRY(); }

    inline TestCase::TestCase(const char* nname, std::function<void()> fxn)
        : name(nname), func(std::move(fxn)) {
        REGISTRY().push_back(this);
    }

    // =========================
    // Assertion failure
    // =========================

    [[noreturn]] inline void FAIL(const char* expr, const char* file, int line,
                                  const char* msg = nullptr) {
        std::ostringstream oss;
        oss << "Assertion failed: " << expr << " (" << file << ":" << line
            << ")";
        if (msg != nullptr) {
            oss << " - " << msg;
        }
        throw std::runtime_error(oss.str());
    }

    inline void EXPECT_FAIL(const std::exception& exc) {
        std::cout << NTEST_COLOR(yellow)("[EXPECT FAIL] ") << exc.what()
                  << "\n";
    }

    inline void EXPECT_FAIL(const std::string& fail) {
        std::cout << NTEST_COLOR(yellow)("[EXPECT FAIL] ") << fail << "\n";
    }

    // =========================
    // Runner
    // =========================

    // Assertion semantics:
    // - REQUIRE / ASSERT abort the current test by throwing
    // - EXPECT logs a failure and continues execution
    // - All exceptions are caught by the runner
    inline int run_all() {
        std::cout << NTEST_COLOR(bold)(NTEST_COLOR(cyan)("NTest Framework\n"))
                  << NTEST_COLOR(cyan)("Running Tests....\n");

        int passed = 0;
        int failed = 0;

        for (auto* test : REGISTRY()) {
            try {
                test->func();
                if (test->expect_fail) {
                    std::cout << "[UNEXPECTED PASS] " << test->name << "\n";
                    ++failed;
                } else {
                    std::cout << NTEST_COLOR(green)(std::string("[PASS] ") +
                                                    test->name)
                              << "\n";
                    ++passed;
                }
            } catch (const std::exception& e) {
                if (test->expect_fail) {
                    EXPECT_FAIL(std::string(test->name));
                    ++passed;
                } else {
                    std::cout
                        << NTEST_COLOR(red)(std::string("[FAIL] ") + test->name)
                        << " - " << NTEST_COLOR(yellow)(e.what()) << "\n";
                    ++failed;
                }
            } catch (...) {
                if (test->expect_fail) {
                    EXPECT_FAIL(std::string(test->name));

                    ++passed;
                } else {
                    std::cout
                        << NTEST_COLOR(red)(std::string("[FAIL] ") + test->name)
                        << " - "
                        << NTEST_COLOR(yellow)("Non-std exception thrown")
                        << "\n";
                    ++failed;
                }
            }
        }

        std::cout << passed << " passed, " << failed << " failed.\n";

        if (failed > 0) {
            std::cout << NTEST_COLOR(bold)(
                NTEST_COLOR(red)("\nTest failed.\n"));
        } else {
            std::cout << NTEST_COLOR(bold)(
                NTEST_COLOR(green)("\nAll tests passed :)\n"));
        }

        return failed;
    }

}  // namespace NTest

// =========================
// Macros
// =========================

#define TEST(name)                                     \
    static void name();                                \
    static NTest::TestCase _ntest_##name(#name, name); \
    static void name()

#define TEST_EXPECT_FAIL(name)                                               \
    static void name();                                                      \
    static NTest::TestCase _ntest_##name(#name, name);                       \
    static int _ntest_expect_##name = (_ntest_##name.expect_fail = true, 0); \
    static void name()

// ----- Fatal assertions -----

#define REQUIRE(cond)                               \
    do {                                            \
        if (!(cond))                                \
            NTest::FAIL(#cond, __FILE__, __LINE__); \
    } while (0)

#define REQUIRE_MSG(cond, msg)                           \
    do {                                                 \
        if (!(cond))                                     \
            NTest::FAIL(#cond, __FILE__, __LINE__, msg); \
    } while (0)

#define REQUIRE_EQ(a, b)                                   \
    do {                                                   \
        const auto& _a = (a);                              \
        const auto& _b = (b);                              \
        if (!(_a == _b))                                   \
            NTest::FAIL(#a " == " #b, __FILE__, __LINE__); \
    } while (0)

#define REQUIRE_NE(a, b)                                   \
    do {                                                   \
        if (!((a) != (b)))                                 \
            NTest::FAIL(#a " != " #b, __FILE__, __LINE__); \
    } while (0)

// ----- Non-fatal assertions -----

#define EXPECT(cond)                        \
    do {                                    \
        try {                               \
            REQUIRE(cond);                  \
        } catch (const std::exception& e) { \
            NTest::EXPECT_FAIL(e);          \
        }                                   \
    } while (0)

#define EXPECT_EQ(a, b)                     \
    do {                                    \
        try {                               \
            REQUIRE_EQ(a, b);               \
        } catch (const std::exception& e) { \
            NTest::EXPECT_FAIL(e);          \
        }                                   \
    } while (0)

// ----- Exception assertions -----

#define REQUIRE_THROW(expr)                                                \
    do {                                                                   \
        bool _threw = false;                                               \
        try {                                                              \
            expr;                                                          \
        } catch (...) {                                                    \
            _threw = true;                                                 \
        }                                                                  \
        if (!_threw)                                                       \
            NTest::FAIL("expected exception: " #expr, __FILE__, __LINE__); \
    } while (0)

#define REQUIRE_NO_THROW(expr)                                               \
    do {                                                                     \
        try {                                                                \
            expr;                                                            \
        } catch (...) {                                                      \
            NTest::FAIL("unexpected exception: " #expr, __FILE__, __LINE__); \
        }                                                                    \
    } while (0)

// ----- Legacy ASSERT_* aliases (fatal) -----

#define ASSERT_TRUE(cond) REQUIRE(cond)
#define ASSERT_FALSE(cond) REQUIRE(!(cond))

#define ASSERT_EQ(a, b) REQUIRE_EQ(a, b)
#define ASSERT_NE(a, b) REQUIRE_NE(a, b)

#define ASSERT_LT(a, b) REQUIRE((a) < (b))
#define ASSERT_GT(a, b) REQUIRE((a) > (b))
#define ASSERT_LE(a, b) REQUIRE((a) <= (b))
#define ASSERT_GE(a, b) REQUIRE((a) >= (b))

#endif  // NTEST_UNIT_H