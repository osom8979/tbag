/**
 * @file   Window.hpp
 * @brief  Window class prototype.
 * @author zer0
 * @date   2019-01-31
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

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gui {

/**
 * Window class prototype.
 *
 * @author zer0
 * @date   2019-01-31
 */
class TBAG_API Window : private Noncopyable
{
public:
    Window();
    virtual ~Window();

public:
    int run();
};

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GUI_WINDOW_HPP__

