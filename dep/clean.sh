#!/bin/bash

## uv
rm -rf uv/.libs
rm -rf uv/_libs
rm -rf uv/src/.libs
rm -rf uv/src/_libs
rm -rf uv/src/unix/.libs
rm -rf uv/src/unix/_libs
rm -rf uv/src/win/.libs
rm -rf uv/src/win/_libs
rm -rf uv/autom4te.cache
rm -rf uv/test

rm -f uv/so_locations
rm -f uv/Makefile
rm -f uv/Makefile.in
rm -f uv/aclocal.m4
rm -f uv/ar-lib
rm -f uv/compile
rm -f uv/config.guess
rm -f uv/config.log
rm -f uv/config.status
rm -f uv/config.sub
rm -f uv/configure
rm -f uv/depcomp
rm -f uv/install-sh
rm -f uv/libtool
rm -f uv/libuv.la
rm -f uv/libuv.pc
rm -f uv/ltmain.sh
rm -f uv/missing

rm -f uv/*.o
rm -f uv/src/*.o
rm -f uv/src/*.lo
rm -f uv/src/unix/*.o
rm -f uv/src/unix/*.lo
rm -f uv/src/win/*.o
rm -f uv/src/win/*.lo
rm -f uv/*.lo

## sqlite3
rm -f sqlite3/*.o
rm -f sqlite3/*.a

## gtest
rm -f gtest/make/*.o
rm -f gtest/make/*.a

