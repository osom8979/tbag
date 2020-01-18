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
#include <libtbag/math/Matrix.hpp>

#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace graphic {

TBAG_API Err convertBLResultToErr(uint32_t code) TBAG_NOEXCEPT;

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
    using Size2d = libtbag::geometry::Size2d;
    using Size2i = libtbag::geometry::Size2i;
    using Matrix3x2d = libtbag::math::BaseMatrix<double, 3, 2>;
    using Matrix = Matrix3x2d;

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

    enum class ContextType
    {
        CT_NONE,
        CT_DUMMY,
        CT_RASTER,
        CT_COUNT,
    };

public:
    struct ImageImpl;
    struct PatternImpl;
    struct GradientImpl;
    struct PathImpl;
    struct FontImpl;
    struct GlyphImpl;

    using SharedImageImpl = std::shared_ptr<ImageImpl>;
    using SharedPatternImpl = std::shared_ptr<PatternImpl>;
    using SharedGradientImpl = std::shared_ptr<GradientImpl>;
    using SharedPathImpl = std::shared_ptr<PathImpl>;
    using SharedFontImpl = std::shared_ptr<FontImpl>;
    using SharedGlyphImpl = std::shared_ptr<GlyphImpl>;

    struct Image
    {
        SharedImageImpl _impl;
        Image();

        Err create(int w, int h, uint32_t format) TBAG_NOEXCEPT;

        int width() const TBAG_NOEXCEPT;
        int height() const TBAG_NOEXCEPT;
        uint32_t format() const TBAG_NOEXCEPT;

        Err readFromFile(char const * file_name) TBAG_NOEXCEPT;
        Err readFromData(void const * data, size_t size) TBAG_NOEXCEPT;

        Err readFromBox(Box const & box);
        Err writeToBox(Box & box) const;
    };

    struct Pattern
    {
        SharedPatternImpl _impl;
        Pattern();
    };

    struct Gradient
    {
        SharedGradientImpl _impl;
        Gradient();
    };

    struct Path
    {
        SharedPathImpl _impl;
        Path();
    };

    struct Font
    {
        SharedFontImpl _impl;
        Font();
    };

    struct Glyph
    {
        SharedGlyphImpl _impl;
        Glyph();
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
    void reset();

public:
    Err init(int width, int height, int channels);

public:
    bool isNone() const TBAG_NOEXCEPT;
    bool equals(Canvas const & obj) const TBAG_NOEXCEPT;

public:
    inline bool operator ==(Canvas const & obj) const TBAG_NOEXCEPT { return equals(obj); }
    inline bool operator !=(Canvas const & obj) const TBAG_NOEXCEPT { return !equals(obj); }

public:
    Size2d getTargetSize() const TBAG_NOEXCEPT;
    double getTargetWidth() const TBAG_NOEXCEPT;
    double getTargetHeight() const TBAG_NOEXCEPT;

public:
    ContextType getContextType() const TBAG_NOEXCEPT;

public:
    Err resetContext() TBAG_NOEXCEPT;

public:
    Err assign(Canvas const & obj) TBAG_NOEXCEPT;

public:
    Err begin(Box & box) TBAG_NOEXCEPT;
    Err end() TBAG_NOEXCEPT;
    Err flush() TBAG_NOEXCEPT;

public:
    struct Cookie { std::uint64_t data[2]; };

public:
    std::size_t getSavedStateCount() const TBAG_NOEXCEPT;
    Err save() TBAG_NOEXCEPT;
    Err save(Cookie & cookie) TBAG_NOEXCEPT;
    Err restore() TBAG_NOEXCEPT;
    Err restore(Cookie const & cookie) TBAG_NOEXCEPT;

public:
    Matrix getMetaMatrix() const TBAG_NOEXCEPT;
    Matrix getUserMatrix() const TBAG_NOEXCEPT;
    Err applyMatrixOperator(uint32_t type, void const * data) TBAG_NOEXCEPT;

    Err setMatrix(Matrix const & m) TBAG_NOEXCEPT;
    Err resetMatrix() TBAG_NOEXCEPT;

    Err translate(double x, double y) TBAG_NOEXCEPT;
    Err translate(Point2i const & p) TBAG_NOEXCEPT;
    Err translate(Point2d const & p) TBAG_NOEXCEPT;
    Err scale(double xy) TBAG_NOEXCEPT;
    Err scale(double x, double y) TBAG_NOEXCEPT;
    Err scale(Point2i const & p) TBAG_NOEXCEPT;
    Err scale(Point2d const & p) TBAG_NOEXCEPT;
    Err skew(double x, double y) TBAG_NOEXCEPT;
    Err skew(Point2d const & p) TBAG_NOEXCEPT;
    Err rotate(double angle) TBAG_NOEXCEPT;
    Err rotate(double angle, double x, double y) TBAG_NOEXCEPT;
    Err rotate(double angle, Point2d const & p) TBAG_NOEXCEPT;
    Err rotate(double angle, Point2i const & p) TBAG_NOEXCEPT;
    Err transform(Matrix const & m) TBAG_NOEXCEPT;

    Err postTranslate(double x, double y) TBAG_NOEXCEPT;
    Err postTranslate(Point2i const & p) TBAG_NOEXCEPT;
    Err postTranslate(Point2d const & p) TBAG_NOEXCEPT;
    Err postScale(double xy) TBAG_NOEXCEPT;
    Err postScale(double x, double y) TBAG_NOEXCEPT;
    Err postScale(Point2i const & p) TBAG_NOEXCEPT;
    Err postScale(Point2d const & p) TBAG_NOEXCEPT;
    Err postSkew(double x, double y) TBAG_NOEXCEPT;
    Err postSkew(Point2d const & p) TBAG_NOEXCEPT;
    Err postRotate(double angle) TBAG_NOEXCEPT;
    Err postRotate(double angle, double x, double y) TBAG_NOEXCEPT;
    Err postRotate(double angle, Point2d const & p) TBAG_NOEXCEPT;
    Err postRotate(double angle, Point2i const & p) TBAG_NOEXCEPT;
    Err postTransform(Matrix const & m) TBAG_NOEXCEPT;

    Err userToMeta() TBAG_NOEXCEPT;

public:
    uint8_t getRenderingQuality() const TBAG_NOEXCEPT;
    uint8_t getGradientQuality() const TBAG_NOEXCEPT;
    uint8_t getPatternQuality() const TBAG_NOEXCEPT;

    Err setRenderingQuality(uint32_t value) TBAG_NOEXCEPT;
    Err setGradientQuality(uint32_t value) TBAG_NOEXCEPT;
    Err setPatternQuality(uint32_t value) TBAG_NOEXCEPT;

public:
    uint32_t getFlattenMode() const TBAG_NOEXCEPT;
    Err setFlattenMode(uint32_t mode) TBAG_NOEXCEPT;

    double getFlattenTolerance() const TBAG_NOEXCEPT;
    Err setFlattenTolerance(double tolerance) TBAG_NOEXCEPT;

public:
    BlendMode getCompositionOperator() const TBAG_NOEXCEPT;
    BlendMode getCompOp() const TBAG_NOEXCEPT
    { return getCompositionOperator(); }

    Err setCompositionOperator(BlendMode mode) TBAG_NOEXCEPT;
    Err setCompOp(BlendMode mode) TBAG_NOEXCEPT
    { return setCompositionOperator(mode); }

public:
    double getGlobalAlpha() const TBAG_NOEXCEPT;
    Err setGlobalAlpha(double alpha) TBAG_NOEXCEPT;

public:
    double getFillAlpha() const TBAG_NOEXCEPT;
    Err setFillAlpha(double alpha) TBAG_NOEXCEPT;

    uint32_t getFillStyleType() const TBAG_NOEXCEPT;
    Err getFillStyle(Rgb32 & out) const TBAG_NOEXCEPT;

    Err setFillStyle(Channel r, Channel g, Channel b, Channel a = channel_max()) TBAG_NOEXCEPT;
    Err setFillStyle(Rgb32 const & color) TBAG_NOEXCEPT;
    Err setFillStyle(Gradient const & gradient) TBAG_NOEXCEPT;
    Err setFillStyle(Pattern const & pattern) TBAG_NOEXCEPT;

    uint32_t getFillRule() const TBAG_NOEXCEPT;
    Err setFillRule(uint32_t rule) TBAG_NOEXCEPT;

public:
    double getStrokeAlpha() const TBAG_NOEXCEPT;
    Err setStrokeAlpha(double alpha) TBAG_NOEXCEPT;

    uint32_t getStrokeStyleType() const TBAG_NOEXCEPT;
    Err getStrokeStyle(Rgb32 & out) const TBAG_NOEXCEPT;

    Err setStrokeStyle(Channel r, Channel g, Channel b, Channel a = channel_max()) TBAG_NOEXCEPT;
    Err setStrokeStyle(Rgb32 const & color) TBAG_NOEXCEPT;
    Err setStrokeStyle(Pattern const & pattern) TBAG_NOEXCEPT;
    Err setStrokeStyle(Gradient const & gradient) TBAG_NOEXCEPT;

    double getStrokeWidth() const TBAG_NOEXCEPT;
    double getStrokeMiterLimit() const TBAG_NOEXCEPT;
    uint32_t getStrokeJoin() const TBAG_NOEXCEPT;
    uint32_t getStrokeStartCap() const TBAG_NOEXCEPT;
    uint32_t getStrokeEndCap() const TBAG_NOEXCEPT;
    double getStrokeDashOffset() const TBAG_NOEXCEPT;
    std::vector<double> getStrokeDashArray() const TBAG_NOEXCEPT;
    uint32_t getStrokeTransformOrder() const TBAG_NOEXCEPT;

    Err setStrokeWidth(double width) TBAG_NOEXCEPT;
    Err setStrokeMiterLimit(double miter_limit) TBAG_NOEXCEPT;
    Err setStrokeJoin(uint32_t stroke_join) TBAG_NOEXCEPT;
    Err setStrokeCap(uint32_t type, uint32_t stroke_cap) TBAG_NOEXCEPT;
    Err setStrokeStartCap(uint32_t stroke_cap) TBAG_NOEXCEPT;
    Err setStrokeEndCap(uint32_t stroke_cap) TBAG_NOEXCEPT;
    Err setStrokeCaps(uint32_t stroke_cap) TBAG_NOEXCEPT;
    Err setStrokeDashOffset(double dash_offset) TBAG_NOEXCEPT;
    Err setStrokeDashArray(std::vector<double> const & dash_array) TBAG_NOEXCEPT;
    Err setStrokeTransformOrder(uint32_t transform_order) TBAG_NOEXCEPT;

public:
    Err restoreClipping() TBAG_NOEXCEPT;
    Err clipToRect(double x, double y, double w, double h) TBAG_NOEXCEPT;
    Err clipToRect(Rect2d const & rect) TBAG_NOEXCEPT;
    Err clipToRect(Rect2i const & rect) TBAG_NOEXCEPT;

public:
    Err clearAll() TBAG_NOEXCEPT;
    Err clearRect(double x, double y, double w, double h) TBAG_NOEXCEPT;
    Err clearRect(Rect2d const & rect) TBAG_NOEXCEPT;
    Err clearRect(Rect2i const & rect) TBAG_NOEXCEPT;

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
    Err strokeBox(double x0, double y0, double x1, double y1) TBAG_NOEXCEPT;
    Err strokeRect(double x, double y, double w, double h) TBAG_NOEXCEPT;
    Err strokeLine(double x0, double y0, double x1, double y1) TBAG_NOEXCEPT;
    Err strokeCircle(double cx, double cy, double r) TBAG_NOEXCEPT;
    Err strokeEllipse(double cx, double cy, double rx, double ry) TBAG_NOEXCEPT;
    Err strokeRoundRect(double x, double y, double w, double h, double r) TBAG_NOEXCEPT;
    Err strokeRoundRect(double x, double y, double w, double h, double rx, double ry) TBAG_NOEXCEPT;
    Err strokeArc(double cx, double cy, double r, double start, double sweep) TBAG_NOEXCEPT;
    Err strokeArc(double cx, double cy, double rx, double ry, double start, double sweep) TBAG_NOEXCEPT;
    Err strokeChord(double cx, double cy, double r, double start, double sweep) TBAG_NOEXCEPT;
    Err strokeChord(double cx, double cy, double rx, double ry, double start, double sweep) TBAG_NOEXCEPT;
    Err strokePie(double cx, double cy, double r, double start, double sweep) TBAG_NOEXCEPT;
    Err strokePie(double cx, double cy, double rx, double ry, double start, double sweep) TBAG_NOEXCEPT;
    Err strokeTriangle(double x0, double y0, double x1, double y1, double x2, double y2) TBAG_NOEXCEPT;
    Err strokePolyline(Point2d const * poly, size_t n) TBAG_NOEXCEPT;
    Err strokePolyline(Point2i const * poly, size_t n) TBAG_NOEXCEPT;
    Err strokePolygon(Point2d const * poly, size_t n) TBAG_NOEXCEPT;
    Err strokePolygon(Point2i const * poly, size_t n) TBAG_NOEXCEPT;
    Err strokeRectArray(Rect2d const * data, size_t n) TBAG_NOEXCEPT;
    Err strokeRectArray(Rect2i const * data, size_t n) TBAG_NOEXCEPT;

public:
    Err blitImage(Point2d const & dst, Box const & src) TBAG_NOEXCEPT;
    Err blitImage(Point2d const & dst, Box const & src, Rect2i const & src_area) TBAG_NOEXCEPT;
    Err blitImage(Point2i const & dst, Box const & src) TBAG_NOEXCEPT;
    Err blitImage(Point2i const & dst, Box const & src, Rect2i const & src_area) TBAG_NOEXCEPT;
    Err blitImage(Rect2d const & dst, Box const & src) TBAG_NOEXCEPT;
    Err blitImage(Rect2d const & dst, Box const & src, Rect2i const & src_area) TBAG_NOEXCEPT;
    Err blitImage(Rect2i const & dst, Box const & src) TBAG_NOEXCEPT;
    Err blitImage(Rect2i const & dst, Box const & src, Rect2i const & src_area) TBAG_NOEXCEPT;

public:
    Err toBox(Box & output) const;
};

} // namespace graphic

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GRAPHIC_CANVAS_HPP__

