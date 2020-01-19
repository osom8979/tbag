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
#include <libtbag/geometry/Arc2.hpp>
#include <libtbag/geometry/Circle2.hpp>
#include <libtbag/geometry/Ellipse2.hpp>
#include <libtbag/geometry/Line2.hpp>
#include <libtbag/geometry/RoundRect2.hpp>
#include <libtbag/geometry/Triangle2.hpp>
#include <libtbag/math/Matrix.hpp>

#include <cstdint>
#include <memory>

#include <string>
#include <vector>

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
    using Arc2d = libtbag::geometry::Arc2d;
    using Circle2d = libtbag::geometry::Circle2d;
    using Ellipse2d = libtbag::geometry::Ellipse2d;
    using Line2d = libtbag::geometry::Line2d;
    using RoundRect2d = libtbag::geometry::RoundRect2d;
    using Triangle2d = libtbag::geometry::Triangle2d;
    using Matrix3x2d = libtbag::math::BaseMatrix<double, 3, 2>;
    using Matrix2x2d = libtbag::math::BaseMatrix<double, 2, 2>;
    using Matrix = Matrix3x2d;
    using FontMatrix = Matrix2x2d;

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

    enum class GeometryDirection
    {
        GD_NONE,
        GD_CW,
        GD_CCW,
    };

    using Gd = GeometryDirection;
    TBAG_CONSTEXPR static GeometryDirection const def_gd = GeometryDirection::GD_CW;

    enum class ExtendMode
    {
        EM_PAD,
        EM_REPEAT,
        EM_REFLECT,
        EM_PAD_X_PAD_Y,
        EM_REPEAT_X_REPEAT_Y,
        EM_REFLECT_X_REFLECT_Y,
        EM_PAD_X_REPEAT_Y,
        EM_PAD_X_REFLECT_Y,
        EM_REPEAT_X_PAD_Y,
        EM_REPEAT_X_REFLECT_Y,
        EM_REFLECT_X_PAD_Y,
        EM_REFLECT_X_REPEAT_Y,
    };

    struct GlyphRun
    {
        void * glyphIdData = nullptr;
        void * placementData = nullptr;
        size_t size = 0;
        uint8_t glyphIdSize = 0;
        uint8_t placementType = 0;
        int8_t glyphIdAdvance = 0;
        int8_t placementAdvance = 0;
        uint32_t flags = 0;
    };

    struct FontMetrics
    {
        float size = 0.0f;
        float ascent = 0.0f;
        float vAscent = 0.0f;
        float descent = 0.0f;
        float vDescent = 0.0f;
        float lineGap = 0.0f;
        float xHeight = 0.0f;
        float capHeight = 0.0f;
        float underlinePosition = 0.0f;
        float underlineThickness = 0.0f;
        float strikethroughPosition = 0.0f;
        float strikethroughThickness = 0.0f;
    };

    struct FontDesignMetrics
    {
        int unitsPerEm = 0;
        int lineGap = 0;
        int xHeight = 0;
        int capHeight = 0;
        int ascent = 0;
        int vAscent = 0;
        int descent = 0;
        int vDescent = 0;
        int hMinLSB = 0;
        int vMinLSB = 0;
        int hMinTSB = 0;
        int vMinTSB = 0;
        int hMaxAdvance = 0;
        int vMaxAdvance = 0;
        int underlinePosition = 0;
        int underlineThickness = 0;
        int strikethroughPosition = 0;
        int strikethroughThickness = 0;
    };

    struct Cookie
    {
        std::uint64_t data[2];
    };

    struct Range
    {
        size_t start = 0;
        size_t end = 0;
    };

    struct GradientStop
    {
        double offset = 0.0;
        Rgb32 rgba;
    };

    struct LinearGradientValues
    {
        double x0 = 0.0;
        double y0 = 0.0;
        double x1 = 0.0;
        double y1 = 0.0;
    };

    struct RadialGradientValues
    {
        double x0 = 0.0;
        double y0 = 0.0;
        double x1 = 0.0;
        double y1 = 0.0;
        double r0 = 0.0;
    };

    struct ConicalGradientValues
    {
        double x0 = 0.0;
        double y0 = 0.0;
        double angle = 0.0;
    };

public:
    struct ImageImpl;
    using SharedImageImpl = std::shared_ptr<ImageImpl>;
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

    struct PatternImpl;
    using SharedPatternImpl = std::shared_ptr<PatternImpl>;
    struct Pattern
    {
        SharedPatternImpl _impl;
        Pattern();

        Err create(Image const & image, ExtendMode extend_mode = ExtendMode::EM_REPEAT) TBAG_NOEXCEPT;
        Err create(Image const & image, ExtendMode extend_mode, Matrix const & m) TBAG_NOEXCEPT;
        Err create(Image const & image, Rect2i const & area, ExtendMode extend_mode = ExtendMode::EM_REPEAT) TBAG_NOEXCEPT;
        Err create(Image const & image, Rect2i const & area, ExtendMode extend_mode, Matrix const & m) TBAG_NOEXCEPT;

        Err setImage(Image const & image) TBAG_NOEXCEPT;
        Err setImage(Image const & image, Rect2i const & area) TBAG_NOEXCEPT;
        Err resetImage() TBAG_NOEXCEPT;

        Rect2i getArea() const TBAG_NOEXCEPT;
        Err setArea(Rect2i const & area) TBAG_NOEXCEPT;
        Err resetArea() TBAG_NOEXCEPT;

        ExtendMode getExtendMode() const TBAG_NOEXCEPT;
        Err setExtendMode(ExtendMode extend_mode) TBAG_NOEXCEPT;
        Err resetExtendMode() TBAG_NOEXCEPT;

        bool hasMatrix() const TBAG_NOEXCEPT;
        uint32_t getMatrixType() const TBAG_NOEXCEPT;
        Matrix getMatrix() const TBAG_NOEXCEPT;

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
    };

    struct GradientImpl;
    using SharedGradientImpl = std::shared_ptr<GradientImpl>;
    struct Gradient
    {
        SharedGradientImpl _impl;
        Gradient();

        Err create(LinearGradientValues const & values, ExtendMode extend_mode = ExtendMode::EM_PAD) TBAG_NOEXCEPT;
        Err create(RadialGradientValues const & values, ExtendMode extend_mode = ExtendMode::EM_PAD) TBAG_NOEXCEPT;
        Err create(ConicalGradientValues const & values, ExtendMode extend_mode = ExtendMode::EM_PAD) TBAG_NOEXCEPT;
        Err create(LinearGradientValues const & values, ExtendMode extend_mode, GradientStop const * stops, size_t n) TBAG_NOEXCEPT;
        Err create(RadialGradientValues const & values, ExtendMode extend_mode, GradientStop const * stops, size_t n) TBAG_NOEXCEPT;
        Err create(ConicalGradientValues const & values, ExtendMode extend_mode, GradientStop const * stops, size_t n) TBAG_NOEXCEPT;
        Err create(LinearGradientValues const & values, ExtendMode extend_mode, GradientStop const * stops, size_t n, Matrix const & m) TBAG_NOEXCEPT;
        Err create(RadialGradientValues const & values, ExtendMode extend_mode, GradientStop const * stops, size_t n, Matrix const & m) TBAG_NOEXCEPT;
        Err create(ConicalGradientValues const & values, ExtendMode extend_mode, GradientStop const * stops, size_t n, Matrix const & m) TBAG_NOEXCEPT;

        uint32_t getType() const TBAG_NOEXCEPT;
        Err setType(uint32_t type) TBAG_NOEXCEPT;

        ExtendMode getExtendMode() const TBAG_NOEXCEPT;
        Err setExtendMode(ExtendMode extend_mode) TBAG_NOEXCEPT;
        Err resetExtendMode() TBAG_NOEXCEPT;
        double getValue(size_t index) const TBAG_NOEXCEPT;

        LinearGradientValues getLinear() const TBAG_NOEXCEPT;
        RadialGradientValues getRadial() const TBAG_NOEXCEPT;
        ConicalGradientValues getConical() const TBAG_NOEXCEPT;

        Err setValue(size_t index, double value) TBAG_NOEXCEPT;
        Err setValues(size_t index, double const * values, size_t n) TBAG_NOEXCEPT;

        Err setValues(LinearGradientValues const & values) TBAG_NOEXCEPT;
        Err setValues(RadialGradientValues const & values) TBAG_NOEXCEPT;
        Err setValues(ConicalGradientValues const & values) TBAG_NOEXCEPT;

        double x0() const TBAG_NOEXCEPT;
        double y0() const TBAG_NOEXCEPT;
        double x1() const TBAG_NOEXCEPT;
        double y1() const TBAG_NOEXCEPT;
        double r0() const TBAG_NOEXCEPT;
        double angle() const TBAG_NOEXCEPT;

        Err setX0(double value) TBAG_NOEXCEPT;
        Err setY0(double value) TBAG_NOEXCEPT;
        Err setX1(double value) TBAG_NOEXCEPT;
        Err setY1(double value) TBAG_NOEXCEPT;
        Err setR0(double value) TBAG_NOEXCEPT;
        Err setAngle(double value) TBAG_NOEXCEPT;

        bool empty() const TBAG_NOEXCEPT;
        size_t size() const TBAG_NOEXCEPT;
        size_t capacity() const TBAG_NOEXCEPT;

        Err reserve(size_t n) TBAG_NOEXCEPT;
        Err shrink() TBAG_NOEXCEPT;

        std::vector<GradientStop> stops() const TBAG_NOEXCEPT;
        GradientStop getStop(size_t i) const TBAG_NOEXCEPT;

        Err resetStops() TBAG_NOEXCEPT;
        Err assignStops(GradientStop const * stops, size_t n) TBAG_NOEXCEPT;
        Err addStop(double offset, Rgb32 const & rgba32) TBAG_NOEXCEPT;
        Err removeStop(size_t index) TBAG_NOEXCEPT;
        Err removeStopByOffset(double offset, bool all = true) TBAG_NOEXCEPT;
        Err removeStops(Range const & range) TBAG_NOEXCEPT;
        Err removeStopsByOffset(double offset_min, double offset_max) TBAG_NOEXCEPT;
        Err replaceStop(size_t index, double offset, Rgb32 const & rgba32) TBAG_NOEXCEPT;
        size_t getIndexOfStop(double offset) const TBAG_NOEXCEPT;

        bool hasMatrix() const TBAG_NOEXCEPT;
        uint32_t getMatrixType() const TBAG_NOEXCEPT;
        Matrix getMatrix() const TBAG_NOEXCEPT;

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
    };

    struct PathImpl;
    using SharedPathImpl = std::shared_ptr<PathImpl>;
    struct Path
    {
        SharedPathImpl _impl;
        Path();

        size_t size() const TBAG_NOEXCEPT;
        size_t capacity() const TBAG_NOEXCEPT;

        Point2d const * vertexData() const TBAG_NOEXCEPT;
        Point2d const * vertexDataEnd() const TBAG_NOEXCEPT;
        uint8_t const * commandData() const TBAG_NOEXCEPT;
        uint8_t const * commandDataEnd() const TBAG_NOEXCEPT;

        Err clear() TBAG_NOEXCEPT;
        Err shrink() TBAG_NOEXCEPT;
        Err reserve(size_t n) TBAG_NOEXCEPT;

        Err modifyOp(uint32_t op, size_t n, uint8_t ** cmd_data_out, Point2d ** vtx_data_out) TBAG_NOEXCEPT;
        Err setVertexAt(size_t index, uint32_t cmd, Point2d const & pt) TBAG_NOEXCEPT;
        Err setVertexAt(size_t index, uint32_t cmd, double x, double y) TBAG_NOEXCEPT;
        Err moveTo(Point2d const & p0) TBAG_NOEXCEPT;
        Err moveTo(double x0, double y0) TBAG_NOEXCEPT;
        Err lineTo(Point2d const & p1) TBAG_NOEXCEPT;
        Err lineTo(double x1, double y1) TBAG_NOEXCEPT;
        Err polyTo(Point2d const * poly, size_t count) TBAG_NOEXCEPT;
        Err quadTo(Point2d const & p1, Point2d const & p2) TBAG_NOEXCEPT;
        Err quadTo(double x1, double y1, double x2, double y2) TBAG_NOEXCEPT;
        Err cubicTo(Point2d const & p1, Point2d const & p2, Point2d const & p3) TBAG_NOEXCEPT;
        Err cubicTo(double x1, double y1, double x2, double y2, double x3, double y3) TBAG_NOEXCEPT;
        Err smoothQuadTo(Point2d const & p2) TBAG_NOEXCEPT;
        Err smoothQuadTo(double x2, double y2) TBAG_NOEXCEPT;
        Err smoothCubicTo(Point2d const & p2, Point2d const & p3) TBAG_NOEXCEPT;
        Err smoothCubicTo(double x2, double y2, double x3, double y3) TBAG_NOEXCEPT;
        Err arcTo(Point2d const & c, Point2d const & r, double start, double sweep, bool force_move_to = false) TBAG_NOEXCEPT;
        Err arcTo(double cx, double cy, double rx, double ry, double start, double sweep, bool force_move_to = false) TBAG_NOEXCEPT;
        Err arcQuadrantTo(Point2d const & p1, Point2d const & p2) TBAG_NOEXCEPT;
        Err arcQuadrantTo(double x1, double y1, double x2, double y2) TBAG_NOEXCEPT;
        Err ellipticArcTo(Point2d const & rp, double x_axis_rotation, bool large_arc_flag, bool sweep_flag, Point2d const & p1) TBAG_NOEXCEPT;
        Err ellipticArcTo(double rx, double ry, double x_axis_rotation, bool large_arc_flag, bool sweep_flag, double x1, double y1) TBAG_NOEXCEPT;
        Err close() TBAG_NOEXCEPT;

        Err addBox(double x0, double y0, double x1, double y1, Gd dir = def_gd) TBAG_NOEXCEPT;
        Err addRect(double x, double y, double w, double h, Gd dir = def_gd) TBAG_NOEXCEPT;
        Err addCircle(Circle2d const & circle, Gd dir = def_gd) TBAG_NOEXCEPT;
        Err addCircle(Circle2d const & circle, Matrix const & m, Gd dir = def_gd) TBAG_NOEXCEPT;
        Err addEllipse(Ellipse2d const & ellipse, Gd dir = def_gd) TBAG_NOEXCEPT;
        Err addEllipse(Ellipse2d const & ellipse, Matrix const & m, Gd dir = def_gd) TBAG_NOEXCEPT;
        Err addRoundRect(RoundRect2d const & rr, Gd dir = def_gd) TBAG_NOEXCEPT;
        Err addRoundRect(RoundRect2d const & rr, Matrix const & m, Gd dir = def_gd) TBAG_NOEXCEPT;
        Err addArc(Arc2d const & arc, Gd dir = def_gd) TBAG_NOEXCEPT;
        Err addArc(Arc2d const & arc, Matrix const & m, Gd dir = def_gd) TBAG_NOEXCEPT;
        Err addChord(Arc2d const & chord, Gd dir = def_gd) TBAG_NOEXCEPT;
        Err addChord(Arc2d const & chord, Matrix const & m, Gd dir = def_gd) TBAG_NOEXCEPT;
        Err addPie(Arc2d const & pie, Gd dir = def_gd) TBAG_NOEXCEPT;
        Err addPie(Arc2d const & pie, Matrix const & m, Gd dir = def_gd) TBAG_NOEXCEPT;
        Err addLine(Line2d const & line, Gd dir = def_gd) TBAG_NOEXCEPT;
        Err addLine(Line2d const & line, Matrix const & m, Gd dir = def_gd) TBAG_NOEXCEPT;
        Err addTriangle(Triangle2d const & triangle, Gd dir = def_gd) TBAG_NOEXCEPT;
        Err addTriangle(Triangle2d const & triangle, Matrix const & m, Gd dir = def_gd) TBAG_NOEXCEPT;

        Err addPolyline(Point2i const * poly, size_t n, Gd dir = def_gd) TBAG_NOEXCEPT;
        Err addPolyline(Point2i const * poly, size_t n, Matrix const & m, Gd dir = def_gd) TBAG_NOEXCEPT;
        Err addPolyline(Point2d const * poly, size_t n, Gd dir = def_gd) TBAG_NOEXCEPT;
        Err addPolyline(Point2d const * poly, size_t n, Matrix const & m, uint32_t dir) TBAG_NOEXCEPT;
        Err addPolygon(Point2i const * poly, size_t n, Gd dir = def_gd) TBAG_NOEXCEPT;
        Err addPolygon(Point2i const * poly, size_t n, Matrix const & m, Gd dir = def_gd) TBAG_NOEXCEPT;
        Err addPolygon(Point2d const * poly, size_t n, Gd dir = def_gd) TBAG_NOEXCEPT;
        Err addPolygon(Point2d const * poly, size_t n, Matrix const & m, Gd dir = def_gd) TBAG_NOEXCEPT;
        Err addRectArray(Rect2i const * data, size_t n, Gd dir = def_gd) TBAG_NOEXCEPT;
        Err addRectArray(Rect2i const * data, size_t n, Matrix const & m, Gd dir = def_gd) TBAG_NOEXCEPT;
        Err addRectArray(Rect2d const * data, size_t n, Gd dir = def_gd) TBAG_NOEXCEPT;
        Err addRectArray(Rect2d const * data, size_t n, Matrix const & m, Gd dir = def_gd) TBAG_NOEXCEPT;
        Err addPath(Path const & path) TBAG_NOEXCEPT;
        Err addPath(Path const & path, Range const & range) TBAG_NOEXCEPT;
        Err addPath(Path const & path, Point2d const & p) TBAG_NOEXCEPT;
        Err addPath(Path const & path, Range const & range, Point2d const & p) TBAG_NOEXCEPT;
        Err addPath(Path const & path, Matrix const & m) TBAG_NOEXCEPT;
        Err addPath(Path const & path, Range const & range, Matrix const & m) TBAG_NOEXCEPT;
        Err addReversedPath(Path const & path, uint32_t reverse_mode) TBAG_NOEXCEPT;
        Err addReversedPath(Path const & path, Range const & range, uint32_t reverse_mode) TBAG_NOEXCEPT;
        Err addStrokedPath(Path const & path, Range const & range) TBAG_NOEXCEPT;
        Err addStrokedPath(Path const & path) TBAG_NOEXCEPT;

        Err translate(Point2d const & p) TBAG_NOEXCEPT;
        Err translate(Range const & range, Point2d const & p) TBAG_NOEXCEPT;
        Err transform(Matrix const & m) TBAG_NOEXCEPT;
        Err transform(Range const & range, Matrix const & m) TBAG_NOEXCEPT;
        Err getControlBox(Rect2d * rect_out) const TBAG_NOEXCEPT;
        Err getBoundingBox(Rect2d * rect_out) const TBAG_NOEXCEPT;
        Err fitTo(Rect2d const & rect, uint32_t fit_flags) TBAG_NOEXCEPT;
        Err fitTo(Range const & range, Rect2d const & rect, uint32_t fit_flags) TBAG_NOEXCEPT;
        Err getInfoFlags(uint32_t * flags_out) const TBAG_NOEXCEPT;
        Err getFigureRange(size_t index, Range * range_out) const TBAG_NOEXCEPT;
        Err getLastVertex(Point2d * vtx_out) TBAG_NOEXCEPT;
        Err getClosestVertex(Point2d const & p, double max_distance, size_t * index_out) const TBAG_NOEXCEPT;
        Err getClosestVertex(Point2d const & p, double max_distance, size_t * index_out, double * distance_out) const TBAG_NOEXCEPT;

        uint32_t hitTest(Point2d const & p, uint32_t fill_rule) const TBAG_NOEXCEPT;
    };

    struct FontLoaderImpl;
    using SharedFontLoaderImpl = std::shared_ptr<FontLoaderImpl>;
    struct FontLoader
    {
        SharedFontLoaderImpl _impl;
        FontLoader();

        Err createFromFile(const char * file_name, uint32_t read_flags = 0) TBAG_NOEXCEPT;
        Err createFromData(const void * data, size_t size) TBAG_NOEXCEPT;

        uint32_t getFaceType() const TBAG_NOEXCEPT;
        uint32_t getFaceCount() const TBAG_NOEXCEPT;
        uint32_t getLoaderFlags() const TBAG_NOEXCEPT;
    };

    struct FontFaceImpl;
    using SharedFontFaceImpl = std::shared_ptr<FontFaceImpl>;
    struct FontFace
    {
        SharedFontFaceImpl _impl;
        FontFace();

        Err createFromFile(char const * file_name, uint32_t read_flags = 0) TBAG_NOEXCEPT;
        Err createFromLoader(FontLoader const & loader, uint32_t face_index) TBAG_NOEXCEPT;

        uint32_t getWeight() const TBAG_NOEXCEPT;
        uint32_t getStretch() const TBAG_NOEXCEPT;
        uint32_t getStyle() const TBAG_NOEXCEPT;

        uint32_t getFaceType() const TBAG_NOEXCEPT;
        uint32_t getOutlineType() const TBAG_NOEXCEPT;
        uint32_t getGlyphCount() const TBAG_NOEXCEPT;

        uint32_t getFaceIndex() const TBAG_NOEXCEPT;
        uint32_t getFaceFlags() const TBAG_NOEXCEPT;
        uint32_t getDiagFlags() const TBAG_NOEXCEPT;
        uint64_t getFaceUniqueId() const TBAG_NOEXCEPT;

        std::string getFullName() const TBAG_NOEXCEPT;
        std::string getFamilyName() const TBAG_NOEXCEPT;
        std::string getSubfamilyName() const TBAG_NOEXCEPT;
        std::string getPostScriptName() const TBAG_NOEXCEPT;

        // FontDesignMetrics designMetrics() const TBAG_NOEXCEPT;
        // int unitsPerEm() const TBAG_NOEXCEPT;
        // FontPanose panose() const TBAG_NOEXCEPT;
        // FontUnicodeCoverage unicodeCoverage() const TBAG_NOEXCEPT;
    };

    struct GlyphBufferImpl;
    using SharedGlyphBufferImpl = std::shared_ptr<GlyphBufferImpl>;
    struct GlyphBuffer
    {
        SharedGlyphBufferImpl _impl;
        GlyphBuffer();

        bool empty() const TBAG_NOEXCEPT;
        size_t size() const TBAG_NOEXCEPT;
        uint32_t flags() const TBAG_NOEXCEPT;

        GlyphRun getGlyphRun() const TBAG_NOEXCEPT;

        bool hasFlag(uint32_t flag) const TBAG_NOEXCEPT;
        bool hasText() const TBAG_NOEXCEPT;
        bool hasGlyphs() const TBAG_NOEXCEPT;
        bool hasInvalidChars() const TBAG_NOEXCEPT;
        bool hasUndefinedChars() const TBAG_NOEXCEPT;
        bool hasInvalidFontData() const TBAG_NOEXCEPT;

        Err reset() TBAG_NOEXCEPT;
        Err clear() TBAG_NOEXCEPT;

        Err setText(void const * data, size_t size, uint32_t encoding) TBAG_NOEXCEPT;
        Err setLatin1Text(char const * data, size_t size = SIZE_MAX) TBAG_NOEXCEPT;
        Err setUtf8Text(char const * data, size_t size = SIZE_MAX) TBAG_NOEXCEPT;
        Err setUtf8Text(uint8_t const * data, size_t size = SIZE_MAX) TBAG_NOEXCEPT;
        Err setUtf16Text(uint16_t const * data, size_t size = SIZE_MAX) TBAG_NOEXCEPT;
        Err setUtf32Text(uint32_t const * data, size_t size = SIZE_MAX) TBAG_NOEXCEPT;
        Err setWCharText(wchar_t const* data, size_t size = SIZE_MAX) TBAG_NOEXCEPT;
        Err setGlyphIds(void const * data, intptr_t advance, size_t size) TBAG_NOEXCEPT;
    };

    struct FontImpl;
    using SharedFontImpl = std::shared_ptr<FontImpl>;
    struct Font
    {
        SharedFontImpl _impl;
        Font();

        Err createFromFace(FontFace const & face, float size) TBAG_NOEXCEPT;

        uint32_t getFaceType() const TBAG_NOEXCEPT;
        uint32_t getFaceFlags() const TBAG_NOEXCEPT;
        float getSize() const TBAG_NOEXCEPT;
        int getUnitsPerEm() const TBAG_NOEXCEPT;
        uint32_t getWeight() const TBAG_NOEXCEPT;
        uint32_t getStretch() const TBAG_NOEXCEPT;
        uint32_t getStyle() const TBAG_NOEXCEPT;
        FontMatrix getMatrix() const TBAG_NOEXCEPT;
        FontMetrics getMetrics() const TBAG_NOEXCEPT;
        FontDesignMetrics getDesignMetrics() const TBAG_NOEXCEPT;
        Err shape(GlyphBuffer & gb) const TBAG_NOEXCEPT;
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
    Err fillPath(Path const & path) TBAG_NOEXCEPT;

    Err fillUtf8Text(Point2i const & dst, Font const & font, char const * text, size_t size = SIZE_MAX) TBAG_NOEXCEPT;
    Err fillUtf8Text(Point2d const & dst, Font const & font, char const * text, size_t size = SIZE_MAX) TBAG_NOEXCEPT;
    Err fillUtf16Text(Point2i const & dst, Font const & font, uint16_t const * text, size_t size = SIZE_MAX) TBAG_NOEXCEPT;
    Err fillUtf16Text(Point2d const & dst, Font const & font, uint16_t const * text, size_t size = SIZE_MAX) TBAG_NOEXCEPT;
    Err fillUtf32Text(Point2i const & dst, Font const & font, uint32_t const * text, size_t size = SIZE_MAX) TBAG_NOEXCEPT;
    Err fillUtf32Text(Point2d const & dst, Font const & font, uint32_t const * text, size_t size = SIZE_MAX) TBAG_NOEXCEPT;
    Err fillGlyphRun(Point2i const & dst, Font const & font, GlyphRun const & glyph_run) TBAG_NOEXCEPT;
    Err fillGlyphRun(Point2d const & dst, Font const & font, GlyphRun const & glyph_run) TBAG_NOEXCEPT;

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
    Err strokePath(Path const & path) TBAG_NOEXCEPT;

    Err strokeUtf8Text(Point2i const & dst, Font const & font, char const * text, size_t size = SIZE_MAX) TBAG_NOEXCEPT;
    Err strokeUtf8Text(Point2d const & dst, Font const & font, char const * text, size_t size = SIZE_MAX) TBAG_NOEXCEPT;
    Err strokeUtf16Text(Point2i const & dst, Font const & font, uint16_t const * text, size_t size = SIZE_MAX) TBAG_NOEXCEPT;
    Err strokeUtf16Text(Point2d const & dst, Font const & font, uint16_t const * text, size_t size = SIZE_MAX) TBAG_NOEXCEPT;
    Err strokeUtf32Text(Point2i const & dst, Font const & font, uint32_t const * text, size_t size = SIZE_MAX) TBAG_NOEXCEPT;
    Err strokeUtf32Text(Point2d const & dst, Font const & font, uint32_t const * text, size_t size = SIZE_MAX) TBAG_NOEXCEPT;
    Err strokeGlyphRun(Point2i const & dst, Font const & font, GlyphRun const & glyph_run) TBAG_NOEXCEPT;
    Err strokeGlyphRun(Point2d const & dst, Font const & font, GlyphRun const & glyph_run) TBAG_NOEXCEPT;

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

