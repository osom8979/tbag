// C++ wrapper for LuaJIT header files.

// Since there is no function declaration inside, a C link is not required.
// [FIX] MSVC error C2894 'templates cannot be declared to have 'C' linkage'
//extern "C" {
#include <libtbag/script/luajit-2.0.5/luaconf.h>
#include <libtbag/script/luajit-2.0.5/lua.h>
#include <libtbag/script/luajit-2.0.5/lualib.h>
#include <libtbag/script/luajit-2.0.5/lauxlib.h>
#include <libtbag/script/luajit-2.0.5/luajit.h>
//}

