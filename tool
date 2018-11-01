#!/usr/bin/env bash

PYTHON_PATH=`which python`
if [[ ! -x ${PYTHON_PATH} ]]; then
    echo 'Not found an executable python.'
    exit 1
fi

function getScriptDirectory {
    local working=$PWD
    cd "$(dirname "${BASH_SOURCE[0]}")"
    echo $PWD
    cd "$working"
}

TOOL_DIR=`getScriptDirectory`
TOOL_PATH=${TOOL_DIR}/tool.py

"${PYTHON_PATH}" "${TOOL_PATH}" $@

