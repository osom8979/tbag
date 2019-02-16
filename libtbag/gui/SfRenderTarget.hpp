/**
 * @file   SfRenderTarget.hpp
 * @brief  SfRenderTarget class prototype.
 * @author zer0
 * @date   2019-02-16
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GUI_SFRENDERTARGET_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GUI_SFRENDERTARGET_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/gui/SfNative.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gui {

/**
 * SfRenderTarget class prototype.
 *
 * @author zer0
 * @date   2019-02-16
 */
class TBAG_API SfRenderTarget : public SfNative
{
public:
    SfRenderTarget(SfType type);
    SfRenderTarget(SfType type, no_init_t);
    virtual ~SfRenderTarget();

public:
    void clear(Channel r, Channel g, Channel b, Channel a = CHANNEL_MAX);
    void clear(Rgb24 const & color);
    void clear(Rgb32 const & color);
    void clear();

//    void setView(const View& view);
//    const View& getView() const;
//    const View& getDefaultView() const;
//    IntRect getViewport(const View& view) const;

//    Vector2f mapPixelToCoords(const Vector2i& point) const;
//    Vector2f mapPixelToCoords(const Vector2i& point, const View& view) const;

//    Vector2i mapCoordsToPixel(const Vector2f& point) const;
//    Vector2i mapCoordsToPixel(const Vector2f& point, const View& view) const;

//    void draw(const Drawable& drawable, const RenderStates& states = RenderStates::Default);
//    void draw(const Vertex* vertices, std::size_t vertexCount, PrimitiveType type, const RenderStates& states = RenderStates::Default);
//    void draw(const VertexBuffer& vertexBuffer, const RenderStates& states = RenderStates::Default);
//    void draw(const VertexBuffer& vertexBuffer, std::size_t firstVertex, std::size_t vertexCount, const RenderStates& states = RenderStates::Default);

    Pointu getSize() const;
    bool setActive(bool active = true);

    void pushGLStates();
    void popGLStates();
    void resetGLStates();
};

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GUI_SFRENDERTARGET_HPP__

