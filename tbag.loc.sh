#!/usr/bin/env bash

echo 'Line of count in tbag project.'

find .     -name "*.cpp"     \
        -o -name "*.hpp"     \
        -o -name "*.c"       \
        -o -name "*.h"       \
        -o -name "*.py"      \
        -o -name "*.simd"    \
        -o -name "*.cmake" | \
    grep -v dep/           | \
    grep -v build/         | \
    grep -v dom/jsoncpp/   | \
    grep -v dom/tinyxml2/  | \
    grep -v string/fmt/    | \
    grep -v external/      | \
    egrep -v "cmake-build-.*/" | \
    xargs wc -l | \
    tail -1

