#!/usr/bin/env sh
set -eu

BUILD_DIR="${BUILD_DIR:-build-test}"

cmake -S . -B "$BUILD_DIR" -G "Unix Makefiles" \
  -DARNICS_BUILD_TESTS=ON \
  -DARNICS_ENABLE_COVERAGE="${ARNICS_ENABLE_COVERAGE:-OFF}"
cmake --build "$BUILD_DIR" --parallel
ctest --test-dir "$BUILD_DIR" --output-on-failure --output-junit "$BUILD_DIR/test-results.xml"
