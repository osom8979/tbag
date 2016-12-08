#!/usr/bin/env bash

function getScriptDirectory {
    local working=$PWD
    cd "$(dirname "${BASH_SOURCE[0]}")"
    echo $PWD
    cd "$working"
}

SCRIPT_PATH=`getScriptDirectory`
DEF_PATH=$SCRIPT_PATH/tbag.cloc
CLOC_PATH=`which cloc`

if [[ ! -x "$CLOC_PATH" ]]; then
    echo "Not found cloc program." 1>&2
    exit 1
fi

"$CLOC_PATH" "--read-lang-def=$DEF_PATH" "--exclude-dir=3rd" \
    "$PWD/libtbag"  \
    "$PWD/test"     \
    "$PWD/tester"   \
    "$PWD/cmake"    \
    "$PWD/tools"    \
    "$PWD/script"   \
    "$PWD/CMakeLists.txt" \
    "$PWD/tool.py"

