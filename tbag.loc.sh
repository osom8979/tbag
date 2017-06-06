#!/usr/bin/env bash

echo 'Line of count in tbag project.'

find . -name "*.cpp" -o -name "*.hpp" -o -name "*.c" -o -name "*.h" -o -name "*.py" -o -name "*.cmake" | grep -v dep/ | grep -v build/ | grep -v 3rd/ | egrep -v "cmake-build-.*/" | grep -v external/ | xargs wc -l | tail -1

