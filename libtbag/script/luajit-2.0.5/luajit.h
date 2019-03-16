// LuaJIT

#ifndef _LUAJIT_H
#define _LUAJIT_H

#include <libtbag/script/luajit-2.0.5/lua.h>

#define LUAJIT_VERSION      "LuaJIT 2.0.5"
#define LUAJIT_VERSION_NUM  20005  /* Version 2.0.5 = 02.00.05. */
#define LUAJIT_VERSION_SYM  luaJIT_version_2_0_5
#define LUAJIT_COPYRIGHT    "Copyright (C) 2005-2017 Mike Pall"
#define LUAJIT_URL          "http://luajit.org/"

// Modes for luaJIT_setmode.
#define LUAJIT_MODE_MASK    0x00ff

enum {
    LUAJIT_MODE_ENGINE,           ///< Set mode for whole JIT engine.
    LUAJIT_MODE_DEBUG,            ///< Set debug mode (idx = level).
    LUAJIT_MODE_FUNC,             ///< Change mode for a function.
    LUAJIT_MODE_ALLFUNC,          ///< Recurse into subroutine protos.
    LUAJIT_MODE_ALLSUBFUNC,       ///< Change only the subroutines.
    LUAJIT_MODE_TRACE,            ///< Flush a compiled trace.
    LUAJIT_MODE_WRAPCFUNC = 0x10, ///< Set wrapper mode for C function calls.
    LUAJIT_MODE_MAX
};

// Flags or'ed in to the mode.
#define LUAJIT_MODE_OFF      0x0000    ///< Turn feature off.
#define LUAJIT_MODE_ON       0x0100    ///< Turn feature on.
#define LUAJIT_MODE_FLUSH    0x0200    ///< Flush JIT-compiled code.

#endif // _LUAJIT_H

