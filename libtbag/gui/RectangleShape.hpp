/**
 * @file   RectangleShape.hpp
 * @brief  RectangleShape class prototype.
 * @author zer0
 * @date   2019-02-10
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GUI_RECTANGLESHAPE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GUI_RECTANGLESHAPE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/gui/Shape.hpp>

#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gui {

/**
 * RectangleShape class prototype.
 *
 * @author zer0
 * @date   2019-02-10
 */
class TBAG_API RectangleShape : private Noncopyable
{
public:
    struct Impl;
    friend struct Impl;

    using UniqueImpl = std::unique_ptr<Impl>;

private:
    UniqueImpl _impl;

public:
    RectangleShape();
    virtual ~RectangleShape();
};

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GUI_RECTANGLESHAPE_HPP__

