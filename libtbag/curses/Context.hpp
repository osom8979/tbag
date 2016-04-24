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
# define LIBTBAG_CURSES_CHAR    chtype
# define LIBTBAG_CURSES_ATTR    attr_t
# define LIBTBAG_CURSES_COLOR   NCURSES_COLOR_T
# define LIBTBAG_CURSES_PAIRS   NCURSES_PAIRS_T
#else // defined(USE_NCURSES)
struct DUMMY_SCREEN { __EMPTY_BLOCK__ };
struct DUMMY_WINDOW { __EMPTY_BLOCK__ };
# define LIBTBAG_CURSES_SCREEN  DUMMY_SCREEN
# define LIBTBAG_CURSES_WINDOW  DUMMY_WINDOW
# define LIBTBAG_CURSES_CHAR    int
# define LIBTBAG_CURSES_ATTR    int
# define LIBTBAG_CURSES_COLOR   short
# define LIBTBAG_CURSES_PAIRS   short
#endif // defined(USE_NCURSES)

// NCurses variables.
#if defined(USE_NCURSES)
# define LIBTBAG_CURSES_STDSCR  stdscr
# define LIBTBAG_CURSES_COLS    COLS
# define LIBTBAG_CURSES_LINES   LINES
#else // defined(USE_NCURSES)
# define LIBTBAG_CURSES_STDSCR  (-1)
# define LIBTBAG_CURSES_COLS    (-1)
# define LIBTBAG_CURSES_LINES   (-1)
#endif // defined(USE_NCURSES)

// NCurses stdscr methods.
#if defined(USE_NCURSES)
# define LIBTBAG_CURSES_INITSCR()                ::initscr() // RETURN WINDOW*
# define LIBTBAG_CURSES_ENDWIN()                 ::endwin()
# define LIBTBAG_CURSES_REFRESH()                ::refresh()
# define LIBTBAG_CURSES_PRINTW(...)              ::printw(__VA_ARGS__)
# define LIBTBAG_CURSES_MVPRINTW(y, x, ...)      ::mvprintw(y, x, __VA_ARGS__)
# define LIBTBAG_CURSES_GETCH()                  ::getch()
# define LIBTBAG_CURSES_GETSTR(str)              ::getstr(str);
# define LIBTBAG_CURSES_WGETNSTR(win, str, size) ::wgetnstr(win, str, size);
# define LIBTBAG_CURSES_CLEAR()                  ::clear()
#else // defined(USE_NCURSES)
# define LIBTBAG_CURSES_INITSCR()                (nullptr)
# define LIBTBAG_CURSES_ENDWIN()                 (-1)
# define LIBTBAG_CURSES_REFRESH()                (-1)
# define LIBTBAG_CURSES_PRINTW(...)              (-1)
# define LIBTBAG_CURSES_MVPRINTW(y, x, ...)      (-1)
# define LIBTBAG_CURSES_GETCH()                  (-1)
# define LIBTBAG_CURSES_GETSTR(str)              (-1)
# define LIBTBAG_CURSES_WGETNSTR(win, str, size) (-1)
# define LIBTBAG_CURSES_CLEAR()                  (-1)
#endif // defined(USE_NCURSES)

// NCurses window methods.
#if defined(USE_NCURSES)
# define LIBTBAG_CURSES_NEWWIN(h, w, y, x)                            ::newwin(h, w, y, x) // RETURN WINDOW*
# define LIBTBAG_CURSES_DELWIN(win)                                   ::delwin(win)
# define LIBTBAG_CURSES_WREFRESH(win)                                 ::wrefresh(win)
# define LIBTBAG_CURSES_BOX(win, vertical, horizontal)                ::box(win, vertical, horizontal)
# define LIBTBAG_CURSES_WBORDER(win, ls, rs, ts, bs, tl, tr, bl, br)  ::wborder(win, ls, rs, ts, bs, tl, tr, bl, br)
# define LIBTBAG_CURSES_MVWPRINTW(win, y, x, ...)                     ::mvwprintw(win, y, x, __VA_ARGS__)
#else // defined(USE_NCURSES)
# define LIBTBAG_CURSES_NEWWIN(h, w, y, x)                            (nullptr)
# define LIBTBAG_CURSES_DELWIN(win)                                   (-1)
# define LIBTBAG_CURSES_WREFRESH(win)                                 (-1)
# define LIBTBAG_CURSES_BOX(win, vertical, horizontal)                (-1)
# define LIBTBAG_CURSES_WBORDER(win, ls, rs, ts, bs, tl, tr, bl, br)  (-1)
# define LIBTBAG_CURSES_MVWPRINTW(win, y, x, ...)                     (-1)
#endif // defined(USE_NCURSES)

// NCurses color methods.
#if defined(USE_NCURSES)
# define LIBTBAG_CURSES_START_COLOR()           ::start_color()
# define LIBTBAG_CURSES_INIT_COLOR(c, r, g, b)  ::init_color(c, r, g, b)
# define LIBTBAG_CURSES_INIT_PAIR(pair, f, b)   ::init_pair(pair, f, b)
#else // defined(USE_NCURSES)
# define LIBTBAG_CURSES_START_COLOR()           (-1)
# define LIBTBAG_CURSES_INIT_COLOR(c, r, g, b)  (-1)
# define LIBTBAG_CURSES_INIT_PAIR(pair, f, b)   (-1)
#endif // defined(USE_NCURSES)

// NCurses terminal methods.
#if defined(USE_NCURSES)
# define LIBTBAG_CURSES_RAW()           ::raw()   // Line buffering disable.
# define LIBTBAG_CURSES_CBREAK()        ::cbreak()
# define LIBTBAG_CURSES_ECHO()          ::echo()
# define LIBTBAG_CURSES_NOECHO()        ::noecho()
# define LIBTBAG_CURSES_ATTRON(v)       ::attron(v)
# define LIBTBAG_CURSES_ATTROFF(v)      ::attroff(v)
# define LIBTBAG_CURSES_KEYPAD(win, b)  ::keypad(win, b)
# define LIBTBAG_CURSES_CURS_SET(flag)  ::curs_set(flag)
#else // defined(USE_NCURSES)
# define LIBTBAG_CURSES_RAW()           (-1)
# define LIBTBAG_CURSES_CBREAK()        (-1)
# define LIBTBAG_CURSES_ECHO()          (-1)
# define LIBTBAG_CURSES_NOECHO()        (-1)
# define LIBTBAG_CURSES_ATTRON(v)       (-1)
# define LIBTBAG_CURSES_ATTROFF(v)      (-1)
# define LIBTBAG_CURSES_KEYPAD(win, b)  (-1)
# define LIBTBAG_CURSES_CURS_SET(flag)  (-1)
#endif // defined(USE_NCURSES)

// NCurses query methods.
#if defined(USE_NCURSES)
# define LIBTBAG_CURSES_GETX(win)           getcurx(win)
# define LIBTBAG_CURSES_GETY(win)           getcury(win)
# define LIBTBAG_CURSES_GETBEGX(win)        getbegx(win)
# define LIBTBAG_CURSES_GETBEGY(win)        getbegy(win)
# define LIBTBAG_CURSES_GETMAXX(win)        getmaxx(win)
# define LIBTBAG_CURSES_GETMAXY(win)        getmaxy(win)
# define LIBTBAG_CURSES_GETPARX(win)        getparx(win)
# define LIBTBAG_CURSES_GETPARY(win)        getpary(win)
# define LIBTBAG_CURSES_GETYX(win, y, x)    getyx   (win, y, x)
# define LIBTBAG_CURSES_GETBEGYX(win, y, x) getbegyx(win, y, x)
# define LIBTBAG_CURSES_GETMAXYX(win, y, x) getmaxyx(win, y, x)
# define LIBTBAG_CURSES_GETPARYX(win, y, x) getparyx(win, y, x)
#else // defined(USE_NCURSES)
# define LIBTBAG_CURSES_GETX(win)           (-1)
# define LIBTBAG_CURSES_GETY(win)           (-1)
# define LIBTBAG_CURSES_GETBEGX(win)        (-1)
# define LIBTBAG_CURSES_GETBEGY(win)        (-1)
# define LIBTBAG_CURSES_GETMAXX(win)        (-1)
# define LIBTBAG_CURSES_GETMAXY(win)        (-1)
# define LIBTBAG_CURSES_GETPARX(win)        (-1)
# define LIBTBAG_CURSES_GETPARY(win)        (-1)
# define LIBTBAG_CURSES_GETYX(win, y, x)    (y = -1, x = -1)
# define LIBTBAG_CURSES_GETBEGYX(win, y, x) (y = -1, x = -1)
# define LIBTBAG_CURSES_GETMAXYX(win, y, x) (y = -1, x = -1)
# define LIBTBAG_CURSES_GETPARYX(win, y, x) (y = -1, x = -1)
#endif // defined(USE_NCURSES)

#if defined(USE_NCURSES)
# define LIBTBAG_CURSES_COLOR_PAIR(n)  COLOR_PAIR(n)
# define LIBTBAG_CURSES_COLOR_BLACK    COLOR_BLACK
# define LIBTBAG_CURSES_COLOR_RED      COLOR_RED
# define LIBTBAG_CURSES_COLOR_GREEN    COLOR_GREEN
# define LIBTBAG_CURSES_COLOR_YELLOW   COLOR_YELLOW
# define LIBTBAG_CURSES_COLOR_BLUE     COLOR_BLUE
# define LIBTBAG_CURSES_COLOR_MAGENTA  COLOR_MAGENTA
# define LIBTBAG_CURSES_COLOR_CYAN     COLOR_CYAN
# define LIBTBAG_CURSES_COLOR_WHITE    COLOR_WHITE
#else // defined(USE_NCURSES)
# define LIBTBAG_CURSES_COLOR_PAIR(n)   (-1)
# define LIBTBAG_CURSES_COLOR_BLACK     0
# define LIBTBAG_CURSES_COLOR_RED       1
# define LIBTBAG_CURSES_COLOR_GREEN     2
# define LIBTBAG_CURSES_COLOR_YELLOW    3
# define LIBTBAG_CURSES_COLOR_BLUE      4
# define LIBTBAG_CURSES_COLOR_MAGENTA   5
# define LIBTBAG_CURSES_COLOR_CYAN      6
# define LIBTBAG_CURSES_COLOR_WHITE     7
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
    using Screen        = LIBTBAG_CURSES_SCREEN;
    using Window        = LIBTBAG_CURSES_WINDOW;
    using CharType      = LIBTBAG_CURSES_CHAR;
    using ColorType     = LIBTBAG_CURSES_COLOR;
    using PairsType     = LIBTBAG_CURSES_PAIRS;
    using AttributeType = LIBTBAG_CURSES_ATTR;

public:
    /**
     * Unsupported NCurses exception class prototype.
     *
     * @author zer0
     * @date   2016-04-24
     */
    struct UnsupportedNcursesException : public std::exception
    {
        static constexpr char const * const MESSAGE = "Does not supported NCurses library.";

        virtual const char * what() const noexcept override {
            return MESSAGE;
        }
    };

public:
    Context() throw(UnsupportedNcursesException) {
        if (!isEnableNcurses()) {
            throw UnsupportedNcursesException();
        }
    }

    ~Context() {
        __EMPTY_BLOCK__
    }

// NCurses stdscr methods.
public:
    Window * initialize() {
        return LIBTBAG_CURSES_INITSCR();
    }

    int release() {
        return LIBTBAG_CURSES_ENDWIN();
    }

    template <typename ... Args>
    int print(std::string const & format, Args ... args) {
        return LIBTBAG_CURSES_PRINTW(format.c_str(), args...);
    }

    int getChar() {
        return LIBTBAG_CURSES_GETCH();
    }

    int getString(char * result, std::size_t buffer_size) {
        return LIBTBAG_CURSES_WGETNSTR(stdscr, result, buffer_size);
    }

    int clear() {
        return LIBTBAG_CURSES_CLEAR();
    }

// Window new/del methods.
public:
    Window * createWindow(int x, int y, int width, int height) {
        return LIBTBAG_CURSES_NEWWIN(height, width, y, x);
    }

    int destroyWindow(Window * window) {
        return LIBTBAG_CURSES_DELWIN(window);
    }

// Update methods.
public:
    int refresh() {
        return LIBTBAG_CURSES_REFRESH();
    }

    int refresh(Window * window) {
        return LIBTBAG_CURSES_WREFRESH(window);
    }

// Window style.
public:
    int setBox(Window * window, CharType vertical, CharType horizontal) {
        return LIBTBAG_CURSES_BOX(window, vertical, horizontal);
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
    int setBorder(Window * window
                       , CharType ls, CharType rs
                       , CharType ts, CharType bs
                       , CharType tl, CharType tr
                       , CharType bl, CharType br) {
        return LIBTBAG_CURSES_WBORDER(window, ls, rs, ts, bs, tl, tr, bl, br);
    }

// Color methods.
public:
    int startColorMode() {
        return LIBTBAG_CURSES_START_COLOR();
    }

    int initColor(ColorType color, ColorType r, ColorType g, ColorType b) {
        return LIBTBAG_CURSES_INIT_COLOR(color, r, g, b);
    }

    int initPair(PairsType pair, ColorType foreground, ColorType background) {
        return LIBTBAG_CURSES_INIT_PAIR(pair, foreground, background);
    }

// Terminal methods.
public:
    int setRaw() {
        return LIBTBAG_CURSES_RAW();
    }

    int setCbreak() {
        return LIBTBAG_CURSES_CBREAK();
    }

    int setEcho() {
        return LIBTBAG_CURSES_ECHO();
    }

    int setNoEcho() {
        return LIBTBAG_CURSES_NOECHO();
    }

// Attribute methods.
public:
    int onAttribute(AttributeType flags) {
        return LIBTBAG_CURSES_ATTRON(flags);
    }

    int offAttribute(AttributeType flags) {
        return LIBTBAG_CURSES_ATTROFF(flags);
    }

public:
    int offAttribute(Window * window, bool enable_function_key) {
        return LIBTBAG_CURSES_KEYPAD(window, enable_function_key);
    }

    int setCursor(int flag) {
        return LIBTBAG_CURSES_CURS_SET(flag);
    }

// Query methods.
public:
    int getCursorX(Window * window) {
        return LIBTBAG_CURSES_GETX(window);
    };
    int getCursorY(Window * window) {
        return LIBTBAG_CURSES_GETY(window);
    };

    int getBeginningX(Window * window) {
        return LIBTBAG_CURSES_GETBEGX(window);
    };
    int getBeginningY(Window * window) {
        return LIBTBAG_CURSES_GETBEGY(window);
    };

    int getMaxX(Window * window) {
        return LIBTBAG_CURSES_GETMAXX(window);
    };
    int getMaxY(Window * window) {
        return LIBTBAG_CURSES_GETMAXY(window);
    };

    int getParentRelativeX(Window * window) {
        return LIBTBAG_CURSES_GETPARX(window);
    };
    int getParentRelativeY(Window * window) {
        return LIBTBAG_CURSES_GETPARY(window);
    };

public:
    std::size_t getColSize() {
        return static_cast<std::size_t>(LIBTBAG_CURSES_COLS);
    }

    std::size_t getRowSize() {
        return static_cast<std::size_t>(LIBTBAG_CURSES_LINES);
    }
};

} // namespace curses

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CURSES_CONTEXT_HPP__

