/**
 * @file   Context.hpp
 * @brief  Context class prototype.
 * @author zer0
 * @date   2016-04-23
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_CURSES_CONTEXT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_CURSES_CONTEXT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/Noncopyable.hpp>

#include <exception>

#if defined(USE_NCURSES)
#include <ncurses/ncurses.h>
#endif

// NCurses Structures.
#if defined(USE_NCURSES)
# define LIBTBAG_CURSES_SCREEN  SCREEN
# define LIBTBAG_CURSES_WINDOW  WINDOW
#else // defined(USE_NCURSES)
struct DUMMY_SCREEN { __EMPTY_BLOCK__ };
struct DUMMY_WINDOW { __EMPTY_BLOCK__ };
# define LIBTBAG_CURSES_SCREEN  DUMMY_SCREEN
# define LIBTBAG_CURSES_WINDOW  DUMMY_WINDOW
#endif // defined(USE_NCURSES)

// NCurses Functions.
#if defined(USE_NCURSES)
# define LIBTBAG_CURSES_INITSCR()               ::initscr()
# define LIBTBAG_CURSES_ENDWIN()                ::endwin()
# define LIBTBAG_CURSES_REFRESH()               ::refresh()
# define LIBTBAG_CURSES_PRINTW(format, ...)     ::printw(format, __VA_ARGS__)
# define LIBTBAG_CURSES_WGETCH(w)               ::wgetch(w)
#else // defined(USE_NCURSES)
# define LIBTBAG_CURSES_INITSCR()               (nullptr)
# define LIBTBAG_CURSES_ENDWIN()                (-1)
# define LIBTBAG_CURSES_REFRESH()               (-1)
# define LIBTBAG_CURSES_PRINTW(format, ...)     (-1)
# define LIBTBAG_CURSES_WGETCH(w)               (-1)
#endif // defined(USE_NCURSES)

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace curses {

constexpr bool isEnableNcurses() noexcept
{
#if defined(USE_NCURSES)
    return true;
#else
    return false;
#endif
}

/**
 * Context class prototype.
 *
 * @author zer0
 * @date   2016-04-23
 */
class Context : public Noncopyable
{
public:
    using Screen = LIBTBAG_CURSES_SCREEN;
    using Window = LIBTBAG_CURSES_WINDOW;

public:
    struct UnsupportedNcursesException : public std::exception
    {
        static constexpr char const * const MESSAGE = "Does not supported Ncurses library.";

        virtual const char * what() const noexcept override {
            return MESSAGE;
        }
    };

private:
    Window * _main = nullptr;

public:
    Context() throw(UnsupportedNcursesException) {
        if (!isEnableNcurses()) {
            throw UnsupportedNcursesException();
        }
    }

    ~Context() {
        release();
    }

public:
    bool isInit() const noexcept {
        return (this->_main != nullptr);
    }

public:
    bool init() {
        this->_main = LIBTBAG_CURSES_INITSCR();
        return isInit();
    }

    void release() {
        if (isInit()) {
            LIBTBAG_CURSES_ENDWIN();
            this->_main = nullptr;
        }
    }

public:
    int refresh() {
        return LIBTBAG_CURSES_REFRESH();
    }

public:
    template <typename ... Args>
    int print(std::string const & format, Args ... args) {
        return LIBTBAG_CURSES_PRINTW(format.c_str(), args...);
    }

    int getChar() {
        return LIBTBAG_CURSES_WGETCH(this->_main);
    }
};

} // namespace curses

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CURSES_CONTEXT_HPP__

