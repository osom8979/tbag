/**
 * @file   Window.hpp
 * @brief  Window class prototype.
 * @author zer0
 * @date   2019-01-31
 * @date   2019-02-13 (Rename: Window -> SfWindow)
 * @date   2019-02-16 (Rename: SfWindow -> RenderWindow)
 * @date   2019-03-23 (Rename: RenderWindow -> Window)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GUI_WINDOW_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GUI_WINDOW_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/res/Storage.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gui {

TBAG_API int runWindow(libtbag::res::Storage & storage);

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GUI_WINDOW_HPP__

