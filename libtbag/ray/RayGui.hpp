/**
 * @file   RayGui.hpp
 * @brief  RayGui class prototype.
 * @author zer0
 * @date   2019-05-06
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_RAY_RAYGUI_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_RAY_RAYGUI_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/ray/RayBypass.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace ray {

TBAG_API bool InitRayGui();
TBAG_API void UpdateRayGui();
TBAG_API void RenderRayGui();
TBAG_API void ShutdownRayGui();

TBAG_API bool GuiBegin(char const * text);
TBAG_API void GuiEnd();

TBAG_API void GuiText(char const * text);

} // namespace ray

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_RAY_RAYGUI_HPP__

