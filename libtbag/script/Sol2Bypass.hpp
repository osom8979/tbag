/**
 * @file   Sol2Bypass.hpp
 * @brief  Sol2Bypass class prototype.
 * @author zer0
 * @date   2018-10-30
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_SCRIPT_SOL2BYPASS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_SCRIPT_SOL2BYPASS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef SOL_USING_CXX_LUA
#define SOL_USING_CXX_LUA 1
#endif

#ifndef SOL_USING_CXX_LUAJIT
#define SOL_USING_CXX_LUAJIT 1
#endif

#ifndef SOL_EXCEPTIONS_SAFE_PROPAGATION
#define SOL_EXCEPTIONS_SAFE_PROPAGATION 1
#endif

#ifndef SOL_CHECK_ARGUMENTS
#define SOL_CHECK_ARGUMENTS 1
#endif

#include <libtbag/script/LuaBypass.hpp>
using namespace libtbag::script;

#include <libtbag/3rd/sol2/sol.hpp>

#endif // __INCLUDE_LIBTBAG__LIBTBAG_SCRIPT_SOL2BYPASS_HPP__

