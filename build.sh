#!/usr/bin/env bash

set -e
LINUX_BUILD="build-linux"
WIN_BUILD="build-windows"
DLLSRC="/usr/x86_64-w64-mingw32/lib"

echo "==> Configuring NTest (Linux)..."
cmake -S . -B $LINUX_BUILD -G Ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \

echo "==> Building NTest for Linux..."
cmake --build $LINUX_BUILD --config Release

echo "==> Running unit tests for NTest (Linux)..."
./build-linux/ntest_selftest

echo "==> Running NTest Example (Linux)..."
./build-linux/example

echo "==> Configuring NTest (Windows)..."
cmake -S . -B $WIN_BUILD -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=cmake/toolchains/mingw64.cmake \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
  -DCMAKE_EXE_LINKER_FLAGS="-static -static-libgcc -static-libstdc++"

echo "==> Building NTest for Windows..."
cmake --build $WIN_BUILD --config Release

echo "==> Running unit tests for NTest (Windows via WINE)..."
wine $WIN_BUILD/ntest_selftest.exe | cat


echo "==> Running NTest Example (Windows via WINE)..."
wine $WIN_BUILD/example.exe | cat

echo "==> Checking formatting..."
tools/format_check.sh