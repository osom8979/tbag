/**
 * @file   Window.hpp
 * @brief  Window class prototype.
 * @author zer0
 * @date   2016-04-27
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_CURSES_WINDOW_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_CURSES_WINDOW_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/curses/Context.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace curses {

/**
 * Window class prototype.
 *
 * @author zer0
 * @date   2016-04-27
 */
class Window : public Noncopyable
{
public:
    using NcursesWindow = typename ::libtbag::curses::Context::NcursesWindow;
    using CharType      = typename ::libtbag::curses::Context::CharType;
    using AttributeType = typename ::libtbag::curses::Context::AttributeType;
    using ColorType     = typename ::libtbag::curses::Context::ColorType;
    using PairType      = typename ::libtbag::curses::Context::PairType;

public:
    using Native = NcursesWindow;

private:
    Context _context;
    Native * _native;

public:
    bool isReady() const noexcept {
        return (this->_native != nullptr ? true : false);
    }

public:
    Window(int x, int y, int w, int h) : _context() {
        this->_native = this->_context.createWindow(x, y, w, h);
    }

    ~Window() {
        if (isReady()) {
            this->_context.destroyWindow(this->_native);
        }
    }

public:
    int getChar() {
        return this->_context.getChar(this->_native);
    }

    int getString(char * result, std::size_t buffer_size) {
        return this->_context.getString(this->_native, result, buffer_size);
    }

// Update methods.
public:
    int update() {
        return this->_context.update(this->_native);
    }

// Window style.
public:
    int setBox(CharType vertical, CharType horizontal) {
        return this->_context.setBox(this->_native, vertical, horizontal);
    }

    /**
     * @param ls [in] left side.
     * @param rs [in] right side.
     * @param ts [in] top side.
     * @param bs [in] bottom side.
     * @param tl [in] top left-hand corner.
     * @param tr [in] top right-hand corner.
     * @param bl [in] bottom left-hand corner.
     * @param br [in] bottom right-hand corner.
     */
    int setBorder(CharType ls, CharType rs
                , CharType ts, CharType bs
                , CharType tl, CharType tr
                , CharType bl, CharType br) {
        return this->_context.setBorder(this->_native, ls, rs, ts, bs, tl, tr, bl, br);
    }

public:
    int setKeypad(bool enable_function_key) {
        return this->_context.setKeypad(this->_native, enable_function_key);
    }

// Attribute ON methods.
public:
    int onAttribute(AttributeType flags) {
        return this->_context.onAttribute(this->_native, flags);
    }

    int onAttribute(PairType pair) {
        return this->_context.onAttribute(this->_native, pair);
    }

// Attribute OFF methods.
public:
    int offAttribute(AttributeType flags) {
        return this->_context.offAttribute(this->_native, flags);
    }

    int offAttribute(PairType pair) {
        return this->_context.offAttribute(this->_native, pair);
    }


public:
    inline int getCursorX() {
        return this->_context.getCursorX(this->_native);
    };

    inline int getCursorY() {
        return this->_context.getCursorY(this->_native);
    };

public:
    inline int getBeginningX() {
        return this->_context.getBeginningX(this->_native);
    };

    inline int getBeginningY() {
        return this->_context.getBeginningY(this->_native);
    };

public:
    inline int getMaxX() {
        return this->_context.getMaxX(this->_native);
    };

    inline int getMaxY() {
        return this->_context.getMaxY(this->_native);
    };

public:
    inline int getParentRelativeX() {
        return this->_context.getParentRelativeX(this->_native);
    };

    inline int getParentRelativeY() {
        return this->_context.getParentRelativeY(this->_native);
    };

public:
    int move(int x, int y) {
        return this->_context.move(this->_native, x, y);
    }

public:
    /**
     * Print single character & move next cursor.
     */
    int addChar(char c, CharType flags = 0) {
        return this->_context.addChar(this->_native, c, flags);
    }

public:
    template <typename ... Args>
    int print(std::string const & format, Args ... args) {
        return this->_context.print(this->_native, format, args...);
    }

public:
    template <typename ... Args>
    int movePrint(int x, int y, std::string const & format, Args ... args) {
        return this->_context.movePrint(this->_native, x, y, format, args...);
    }
};

} // namespace curses

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CURSES_WINDOW_HPP__

