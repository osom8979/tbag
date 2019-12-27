/**
 * @file   LuaMain.hpp
 * @brief  LuaMain class prototype.
 * @author zer0
 * @date   2019-01-30
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_SCRIPT_LUAMAIN_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_SCRIPT_LUAMAIN_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace script {

TBAG_API int runLuaJit(int argc, char ** argv);

} // namespace script

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_SCRIPT_LUAMAIN_HPP__

