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
class Window : public Context
{
public:
    using NcursesWindow = typename Context::NcursesWindow;
    using CharType      = typename Context::CharType;
    using AttributeType = typename Context::AttributeType;
    using ColorType     = typename Context::ColorType;
    using PairType      = typename Context::PairType;

public:
    using Native = NcursesWindow;

private:
    Native * _native;

public:
    bool isReady() const noexcept {
        return (this->_native != nullptr ? true : false);
    }

public:
    Window() : _native(nullptr) {
        __EMPTY_BLOCK__
    }

    Window(int x, int y, int w, int h) : Window() {
        create(x, y, w, h);
    }

    virtual ~Window() {
        destroy();
    }

public:
    bool create(int x, int y, int w, int h) {
        this->_native = Context::createWindow(x, y, w, h);
        return isReady();
    }

    void destroy() {
        if (isReady()) {
            Context::destroyWindow(this->_native);
            this->_native = nullptr;
        }
    }

protected:
    inline Native * getNative() noexcept {
        return this->_native;
    }

    inline void setNative(Native * native) noexcept {
        this->_native = native;
    }

public:
    int getChar() {
        return Context::getChar(this->_native);
    }

    int getString(char * result, std::size_t buffer_size) {
        return Context::getString(this->_native, result, buffer_size);
    }

// Update methods.
public:
    int update() {
        return Context::update(this->_native);
    }

// Window style.
public:
    int setBox(CharType vertical, CharType horizontal) {
        return Context::setBox(this->_native, vertical, horizontal);
    }

    int setBorder(CharType ls, CharType rs
                , CharType ts, CharType bs
                , CharType tl, CharType tr
                , CharType bl, CharType br) {
        return Context::setBorder(this->_native, ls, rs, ts, bs, tl, tr, bl, br);
    }

public:
    int setKeypad(bool enable_function_key) {
        return Context::setKeypad(this->_native, enable_function_key);
    }

// Attribute ON methods.
public:
    int onAttribute(AttributeType flags) {
        return Context::onAttribute(this->_native, flags);
    }

    int onAttribute(PairType pair) {
        return Context::onAttribute(this->_native, pair);
    }

    int onColorAttribute(int pair_number) {
        return Context::onColorAttribute(this->_native, pair_number);
    }

// Attribute OFF methods.
public:
    int offAttribute(AttributeType flags) {
        return Context::offAttribute(this->_native, flags);
    }

    int offAttribute(PairType pair) {
        return Context::offAttribute(this->_native, pair);
    }

    int offColorAttribute(int pair_number) {
        return Context::offColorAttribute(this->_native, pair_number);
    }

public:
    inline int getCursorX() {
        return Context::getCursorX(this->_native);
    }

    inline int getCursorY() {
        return Context::getCursorY(this->_native);
    }

public:
    inline int getBeginningX() {
        return Context::getBeginningX(this->_native);
    }

    inline int getBeginningY() {
        return Context::getBeginningY(this->_native);
    }

public:
    inline int getMaxX() {
        return Context::getMaxX(this->_native);
    }

    inline int getMaxY() {
        return Context::getMaxY(this->_native);
    }

public:
    inline int getParentRelativeX() {
        return Context::getParentRelativeX(this->_native);
    }

    inline int getParentRelativeY() {
        return Context::getParentRelativeY(this->_native);
    }

public:
    int move(int x, int y) {
        return Context::move(this->_native, x, y);
    }

public:
    /**
     * Print single character & move next cursor.
     */
    int addChar(char c, CharType flags = 0) {
        return Context::addChar(this->_native, c, flags);
    }

public:
    template <typename ... Args>
    int print(std::string const & format, Args ... args) {
        return Context::print(this->_native, format, args...);
    }

public:
    template <typename ... Args>
    int movePrint(int x, int y, std::string const & format, Args ... args) {
        return Context::movePrint(this->_native, x, y, format, args...);
    }
};

} // namespace curses

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CURSES_WINDOW_HPP__

