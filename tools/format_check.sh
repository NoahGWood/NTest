#!/usr/bin/env bash
set -euo pipefail

FILES=$(git ls-files \
  'src/*.cpp' \
  'include/*.h' \
  'tests/*.cpp' \
)

if [ -z "$FILES" ]; then
  echo "No files to format-check."
  exit 0
fi

clang-format --dry-run --Werror $FILES
