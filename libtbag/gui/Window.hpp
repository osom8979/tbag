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
#include <libtbag/gui/WindowInterface.hpp>
#include <libtbag/gui/WindowParams.hpp>

#include <libtbag/graphic/Color.hpp>
#include <libtbag/geometry/GeometryTypes.hpp>

#include <string>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gui {

/**
 * Window class prototype.
 *
 * @author zer0
 * @date   2019-01-31
 * @date   2019-02-13 (Rename: Window -> SfWindow)
 * @date   2019-02-16 (Rename: SfWindow -> RenderWindow)
 * @date   2019-03-23 (Rename: RenderWindow -> Window)
 */
class TBAG_API Window : public WindowInterface,
                        public libtbag::geometry::GeometryTypes
{
public:
    using Channel = libtbag::graphic::Channel;
    using Rgb24   = libtbag::graphic::Rgb24;
    using Rgb32   = libtbag::graphic::Rgb32;

public:
    TBAG_CONSTEXPR static Channel const CHANNEL_MAX  = libtbag::graphic::channel_max();
    TBAG_CONSTEXPR static Channel const CHANNEL_MIN  = libtbag::graphic::channel_min();
    TBAG_CONSTEXPR static Channel const CHANNEL_HALF = libtbag::graphic::channel_half();

public:
    struct Backend;
    friend struct Backend;

public:
    using UniqueBackend = std::unique_ptr<Backend>;

private:
    UniqueBackend _impl;

public:
    Window();
    Window(WindowParams const & params);
    virtual ~Window();

public:
    bool pollEvent();
    bool waitEvent();

public:
    virtual void onClosed() override;

public:
    void close();
    bool isOpen() const;

public:
    Pointi getPosition() const;
    void setPosition(Pointi const & position);
    void setSize(Sizeu const & size);

public:
    void setTitle(std::string const & title);
    void setIcon(unsigned int width, unsigned int height, std::uint8_t const * pixels);
    void setVisible(bool visible);
    void setVerticalSyncEnabled(bool enabled);
    void setMouseCursorVisible(bool visible);
    void setMouseCursorGrabbed(bool grabbed);
    void setKeyRepeatEnabled(bool enabled);
    void setFramerateLimit(unsigned int limit);
    void setJoystickThreshold(float threshold);

public:
    //void setMouseCursor(Cursor const & cursor);

public:
    void requestFocus();
    bool hasFocus() const;

public:
    void display();

public:
    void clear(Channel r, Channel g, Channel b, Channel a = CHANNEL_MAX);
    void clear(Rgb24 const & color);
    void clear(Rgb32 const & color);
    void clear();

public:
    Pointf mapPixelToCoords(Pointi const & point) const;
    Pointi mapCoordsToPixel(Pointf const & point) const;

public:
    Sizeu getSize() const;
    bool  setActive(bool active = true);

public:
    void pushGLStates();
    void popGLStates();
    void resetGLStates();
};

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GUI_WINDOW_HPP__

