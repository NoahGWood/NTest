/**
 * @file NTest.h
 * @author NoahGWood
 * @brief Single-header include for the NTest framework
 * @version 0.1
 * @date 2025-4-15
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef NTEST_UNIT_H
#define NTEST_UNIT_H

#include <functional>
#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>
#include "Colors.h" // Inluded from NCLI lib

namespace NTest {

    struct TestCase;


    inline std::vector<TestCase*>& get_registered_tests() {
        static std::vector<TestCase*> tests;
        return tests;
    }

    struct TestCase {
        const char* name;
        std::function<void()> func;

        TestCase(const char* n, std::function<void()>f) : name(n), func(f) {
            get_registered_tests().push_back(this);
        }
    };


    inline int run_all() {
        std::cout << NCLI::Color::bold(NCLI::Color::cyan("NTest Framework\n"))
            << NCLI::Color::cyan("Running Tests....\n");
        int passed = 0, failed = 0;
        for (auto* test : get_registered_tests()) {
            try {
                test->func();
                std::cout << NCLI::Color::green(std::string("[PASS] ") + test->name) << "\n";
                passed++;
            } catch (const std::exception& e) {
                std::cout << NCLI::Color::red(std::string("[FAIL] ") + test->name) << " - " << NCLI::Color::yellow(e.what()) << "\n";
                failed++;
            }
        }
        std::cout << passed << " passed, " << failed << " failed.\n";
        if(failed > 0){
            std::cout << NCLI::Color::bold(NCLI::Color::red("\nTest failed.\n"));
        } else {
            std::cout << NCLI::Color::bold(NCLI::Color::green("\nAll tests passed :)\n"));
        }
        return failed;
    }
}

#define TEST(name) \
    static void name(); \
    static NTest::TestCase _##name(#name, (void (*)())name); \
    static void name()

#define ASSERT_TRUE(cond) \
    if(!(cond)) throw std::runtime_error("Assertion failed: " #cond)


#define ASSERT_FALSE(cond) \
    if(cond) throw std::runtime_error("Assertion failed: " #cond)


#define ASSERT_EQ(a, b) \
    if((a) != (b)) throw std::runtime_error("Assertion failed: " #a " == " #b)


#define ASSERT_NE(a, b) \
    if((a) == (b)) throw std::runtime_error("Assertion failed: " #a " != " #b)


#define ASSERT_LT(a, b) \
    if((a) >= (b)) throw std::runtime_error("Assertion failed: " #a " < " #b)


#define ASSERT_GT(a, b) \
    if((a) <= (b)) throw std::runtime_error("Assertion failed: " #a " > " #b)


#define ASSERT_LE(a, b) \
    if((a) > (b)) throw std::runtime_error("Assertion failed: " #a " < " #b)


#define ASSERT_GE(a, b) \
    if((a) < (b)) throw std::runtime_error("Assertion failed: " #a " >= " #b)


#define ASSERT_THROW(code)                                  \
    do {                                                    \
        bool threw = false;                                 \
        try { code; }                                       \
        catch (...) { threw = true; }                       \
        if (!threw) throw std::runtime_error(               \
            "Assertion failed: expected exception thrown"); \
    } while (0)

#define EXPECT_EQ(a, b)                                     \
    try { ASSERT_EQ(a, b); }                                \
    catch (const std::exception& e) {                       \
        std::cerr << "[EXPECT FAIL] " << e.what() << "\n";  \
    }

#define EXPECT_TRUE(cond)                                   \
    try { ASSERT_TRUE(cond); }                              \
    catch (const std::exception& e) {                       \
        std::cerr << "[EXPECT FAIL] " << e.what() << "\n";  \
    }

#endif // Define NTEST_UNIT_H