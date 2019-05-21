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

if [[ ! -f "$PREFIX/bin/tpot$EXE_SUFFIX" ]]; then
    echo "Not found $PREFIX/bin/tpot$EXE_SUFFIX"
    exit 1
fi
if [[ ! -f "$PREFIX/lib/libtbag$LIB_SUFFIX" ]]; then
    echo "Not found $PREFIX/lib/libtbag$LIB_SUFFIX"
    exit 1
fi

if [[ ! -h "$PREFIX/lib/liblua$LIB_SUFFIX" ]]; then
    ln -s "$PREFIX/lib/libtbag$LIB_SUFFIX" "$PREFIX/lib/liblua$LIB_SUFFIX"
fi
if [[ ! -h "$PREFIX/bin/luajit$EXE_SUFFIX" ]]; then
    ln -s "$PREFIX/bin/tpot$EXE_SUFFIX" "$PREFIX/bin/luajit$EXE_SUFFIX"
fi

INSTALL_DIR=$LUAROCKS_PREFIX
LIBRARY_DIR=$PREFIX/lib

cd "$SOURCE_DIR"
./configure --prefix=$INSTALL_DIR \
    --with-lua=$PREFIX \
    --with-lua-bin=$PREFIX/bin \
    --with-lua-include=$PREFIX/include \
    --with-lua-lib=$PREFIX/lib \
    --with-lua-interpreter=$PREFIX/bin/luajit$EXE_SUFFIX \
    --rocks-tree=$INSTALL_DIR \
    --force-config
make
make install
cd "$WORKING_DIR"

echo 'Install done.'

