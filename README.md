# NTest – Minimal C++ Unit Testing Framework

[![CI](https://github.com/NoahGWood/NTest/actions/workflows/ci.yaml/badge.svg)](https://github.com/NoahGWood/NTest/actions/workflows/ci.yaml)&nbsp;[![Issues](https://img.shields.io/github/issues/NoahGWood/NTest.svg)](https://github.com/NoahGWood/NTest/issues)&nbsp;[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)]()&nbsp;[![Last Commit](https://img.shields.io/github/last-commit/NoahGWood/NTest.svg)](https://github.com/NoahGWood/NTest)

**NTest** is a lightweight, two-header unit testing framework for C++17 projects.  
Minimal, dependency-free, and CI-friendly.

---

## Features

- Dual-header design
- Automatic test registration
- Fatal (`REQUIRE`) and non-fatal (`EXPECT`) assertions
- Expected-failure support (for framework self-testing)
- Robust exception handling (std + non-std)
- ANSI-colored terminal output (no external dependencies)
- Cross-platform: Linux + Windows (MinGW-tested)

---

## Assertion Semantics

NTest follows a simple model:

| Macro      | Behavior                                     |
| ---------- | -------------------------------------------- |
| `REQUIRE`  | Fails the current test immediately (fatal)   |
| `ASSERT_*` | Alias for `REQUIRE` (backward compatibility) |
| `EXPECT`   | Logs failure but continues test execution    |

All exceptions thrown inside tests are caught by the runner.
Failures never stop the test suite.

---

## Basic Usage

```cpp
#define NTEST_MAIN
#include "NTest.h"

TEST(MyFirstTest) {
    REQUIRE(1 + 1 == 2);
    EXPECT(false);  // logs but continues
}
```

Compile and run:

```sh
g++ -std=c++17 my_tests.cpp -o tests
./tests
```

------

## Expected-Fail Support (Framework Testing)

NTest supports marking tests as expected to fail:

```cpp
TEST_EXPECT_FAIL(MyFailureTest) {
    REQUIRE(false);
}
```

Expected failures do ***not*** cause the test run to fail in CI.

------

## Example Output

![example.png](./images/example.png)

------

## CI / Tooling

- `compile_commands.json` exported for clang tooling
- Linux and Windows builds validated via CI
- MinGW cross-compilation supported on Linux

------

## Philosophy

NTest aims to remain:

- Header-only
- Deterministic
- Minimal
- Easy to vendor
- CI-stable

It intentionally avoids advanced meta-programming and heavy DSLs.


---

## License

[TBD]

