#!/bin/bash

echo "Update tparty library: start."

WORKING=$PWD
pushd $WORKING

# Check .git directory.
if [[ ! -d "$TPARTY_HOME/.git" ]]; then
    rm -rf "$TPARTY_HOME"
fi

# Clone or update.
if [[ -d "$TPARTY_HOME/.git" ]]; then
    cd "$TPARTY_HOME" && git pull --all
else
    git clone https://github.com/osom8979/tparty.git $TPARTY_HOME
fi

# Remove cache.
if [[ -f "$TPARTY_HOME/cache" ]]; then
    rm -f "$TPARTY_HOME/cache"
fi

# Install.
cd "$TPARTY_HOME"
echo 'gtest.sh'     > cache
echo 'lua.sh'      >> cache
./install.sh -y

# Remove build dir.
if [[ -d "$TPARTY_HOME/tmp/build" ]]; then
    rm -rf "$TPARTY_HOME/tmp/build"
fi

popd
echo "Update tparty library: done."

