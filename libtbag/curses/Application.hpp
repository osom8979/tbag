/**
 * @file   Application.hpp
 * @brief  Application class prototype.
 * @author zer0
 * @date   2016-04-27
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_CURSES_APPLICATION_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_CURSES_APPLICATION_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/curses/Window.hpp>

#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace curses {

/**
 * Application class prototype.
 *
 * @author zer0
 * @date   2016-04-27
 */
class Application : public Window
{
public:
    Application() {
        setNative(Context::initialize());
        assert(getNative() == Context::getStandardWindow());
    }

    virtual ~Application() {
        Context::release();
        setNative(nullptr);
    }

private:
    bool create(int x, int y, int w, int h) = delete;
    void destroy() = delete;

public:
    void clear() {
        Context::clear();
    }

// Terminal methods.
public:
    void setRaw() {
        Context::setRaw();
    }

    void setCbreak() {
        Context::setCbreak();
    }

    void setEcho() {
        Context::setEcho();
    }

    void setNoecho() {
        Context::setNoecho();
    }

    void setCursor(int flag) {
        Context::setCursor(flag);
    }

public:
    inline int getTerminalWidth() {
        return Context::getTerminalWidth();
    }

    inline int getTerminalHeight() {
        return Context::getTerminalHeight();
    }
};

} // namespace curses

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CURSES_APPLICATION_HPP__

