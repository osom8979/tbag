/**
 * @file   Canvas.hpp
 * @brief  Canvas class prototype.
 * @author zer0
 * @date   2020-01-11
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GRAPHIC_CANVAS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GRAPHIC_CANVAS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/box/Box.hpp>
#include <libtbag/graphic/Color.hpp>
#include <libtbag/geometry/Point2.hpp>
#include <libtbag/geometry/Rect2.hpp>

#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace graphic {

/**
 * Canvas class prototype.
 *
 * @author zer0
 * @date   2020-01-11
 */
class TBAG_API Canvas : private Noncopyable
{
public:
    using Box = libtbag::box::Box;
    using Point2d = libtbag::geometry::Point2d;
    using Point2i = libtbag::geometry::Point2i;
    using Rect2d = libtbag::geometry::Rect2d;
    using Rect2i = libtbag::geometry::Rect2i;

public:
    struct Impl;
    using UniqueImpl = std::unique_ptr<Impl>;

public:
    enum class BlendMode
    {
        BM_SRC_OVER, // [default]
        BM_SRC_COPY,
        BM_SRC_IN,
        BM_SRC_OUT,
        BM_SRC_ATOP,
        BM_DST_OVER,
        BM_DST_COPY, // [nop]
        BM_DST_IN,
        BM_DST_OUT,
        BM_DST_ATOP,
        BM_XOR,
        BM_CLEAR,
        BM_PLUS,
        BM_MINUS,
        BM_MULTIPLY,
        BM_SCREEN,
        BM_OVERLAY,
        BM_DARKEN,
        BM_LIGHTEN,
        BM_COLOR_DODGE,
        BM_COLOR_BURN,
        BM_LINEAR_BURN,
        BM_LINEAR_LIGHT,
        BM_PIN_LIGHT,
        BM_HARD_LIGHT,
        BM_SOFT_LIGHT,
        BM_DIFFERENCE,
        BM_EXCLUSION,
    };

private:
    UniqueImpl _impl;

public:
    Canvas();
    Canvas(int width, int height, int channels);
    ~Canvas();

public:
    inline bool exists() const TBAG_NOEXCEPT
    {
        return static_cast<bool>(_impl);
    }

public:
    static Err convertBLResultToErr(uint32_t code) TBAG_NOEXCEPT;

public:
    void reset();

public:
    Err init(int width, int height, int channels);
    Err end() TBAG_NOEXCEPT;

public:
    Err setCompositionOperator(BlendMode mode) TBAG_NOEXCEPT;
    Err setCompOp(BlendMode mode) TBAG_NOEXCEPT
    { return setCompositionOperator(mode); }

public:
    Err setFillStyle(Channel r, Channel g, Channel b, Channel a = channel_max()) TBAG_NOEXCEPT;
    Err setFillStyle(Rgb32 const & color) TBAG_NOEXCEPT;

public:
    Err fillAll() TBAG_NOEXCEPT;
    Err fillBox(double x0, double y0, double x1, double y1) TBAG_NOEXCEPT;
    Err fillRect(double x, double y, double w, double h) TBAG_NOEXCEPT;
    Err fillCircle(double cx, double cy, double r) TBAG_NOEXCEPT;
    Err fillEllipse(double cx, double cy, double rx, double ry) TBAG_NOEXCEPT;
    Err fillRoundRect(double x, double y, double w, double h, double r) TBAG_NOEXCEPT;
    Err fillRoundRect(double x, double y, double w, double h, double rx, double ry) TBAG_NOEXCEPT;
    Err fillChord(double cx, double cy, double r, double start, double sweep) TBAG_NOEXCEPT;
    Err fillChord(double cx, double cy, double rx, double ry, double start, double sweep) TBAG_NOEXCEPT;
    Err fillPie(double cx, double cy, double r, double start, double sweep) TBAG_NOEXCEPT;
    Err fillPie(double cx, double cy, double rx, double ry, double start, double sweep) TBAG_NOEXCEPT;
    Err fillTriangle(double x0, double y0, double x1, double y1, double x2, double y2) TBAG_NOEXCEPT;
    Err fillPolygon(Point2d const * poly, size_t n) TBAG_NOEXCEPT;
    Err fillPolygon(Point2i const * poly, size_t n) TBAG_NOEXCEPT;
    Err fillRectArray(Rect2d const * data, size_t n) TBAG_NOEXCEPT;
    Err fillRectArray(Rect2i const * data, size_t n) TBAG_NOEXCEPT;

public:
    Err toBox(libtbag::box::Box & output) const;
};

} // namespace graphic

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GRAPHIC_CANVAS_HPP__

