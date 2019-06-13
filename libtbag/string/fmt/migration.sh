#!/usr/bin/env bash

find . -name '*.h' -o -name '*.cc' | \
    xargs sed -i .tmp 's/^#include "fmt\/\(.*\)"$/#include <libtbag\/string\/fmt\/\1>/g'
find . -name '*.h' -o -name '*.cc' | \
    xargs sed -i .tmp 's/^#include "\(.*\)"$/#include <libtbag\/string\/fmt\/\1>/g'
find . -name '*.tmp' | xargs rm

