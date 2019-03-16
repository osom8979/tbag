// Standard library header.

#ifndef _LUALIB_H
#define _LUALIB_H

#include <libtbag/script/luajit-2.0.5/lua.h>

#define LUA_FILEHANDLE   "FILE*"
#define LUA_COLIBNAME    "coroutine"
#define LUA_MATHLIBNAME  "math"
#define LUA_STRLIBNAME   "string"
#define LUA_TABLIBNAME   "table"
#define LUA_IOLIBNAME    "io"
#define LUA_OSLIBNAME    "os"
#define LUA_LOADLIBNAME  "package"
#define LUA_DBLIBNAME    "debug"
#define LUA_BITLIBNAME   "bit"
#define LUA_JITLIBNAME   "jit"
#define LUA_FFILIBNAME   "ffi"

#ifndef lua_assert
#define lua_assert(x) ((void)0)
#endif

#endif // _LUALIB_H

