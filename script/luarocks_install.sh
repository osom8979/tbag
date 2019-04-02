#!/usr/bin/env bash

function getScriptDirectory {
    local working=$PWD
    cd "$(dirname "${BASH_SOURCE[0]}")"
    echo $PWD
    cd "$working"
}

WORKING_DIR=`getScriptDirectory`

DOWNLOAD_URL="http://luarocks.github.io/luarocks/releases/luarocks-3.0.4.tar.gz"
FILENAME="luarocks-3.0.4.tar.gz"
SOURCE_DIR="luarocks-3.0.4"

if [[ ! -f "$FILENAME" ]]; then
    curl -o "$FILENAME" "$DOWNLOAD_URL"
fi
if [[ ! -d "$SOURCE_DIR" ]]; then
    tar xzf "$FILENAME"
fi

case "$(uname -s)" in
Darwin)
    LIB_SUFFIX=.dylib
    ;;
Linux)
    LIB_SUFFIX=.so
    ;;
*)
    echo 'Unsupported platform.'
    ;;
esac

if [[ ! -h "liblua$LIB_SUFFIX" ]]; then
    ln -s "libtbag$LIB_SUFFIX" "liblua$LIB_SUFFIX"
fi
if [[ ! -h "luajit" ]]; then
    ln -s "tpot" "luajit"
fi
if [[ ! -d "$WORKING_DIR/storage/luarocks" ]]; then
    mkdir -p "$WORKING_DIR/storage/luarocks"
fi

INSTALL_DIR="$WORKING_DIR/storage/luarocks"
INCLUDE_DIR="$WORKING_DIR/external/local/include"
LIBRARY_DIR="$WORKING_DIR"

cd "$SOURCE_DIR"
./configure --prefix=$INSTALL_DIR \
    --with-lua=$WORKING_DIR \
    --with-lua-bin=$WORKING_DIR \
    --with-lua-include=$INCLUDE_DIR \
    --with-lua-lib=$LIBRARY_DIR \
    --with-lua-interpreter=tpot \
    --rocks-tree=$INSTALL_DIR \
    --force-config
make
make install
cd "$WORKING_DIR"

echo 'Install done.'

