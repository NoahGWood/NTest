#!/usr/bin/env bash
set -e
cmake -B build -G Ninja .
cmake --build build
./build/example
./build/ntest_selftest