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

#include <ncurses/ncurses.h>

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

/**
 * Context class prototype.
 *
 * @author zer0
 * @date   2016-04-23
 */
class Context : public Noncopyable
{
public:
    using NcursesScreen = SCREEN;
    using NcursesWindow = WINDOW;
    using CharType      = chtype;
    using AttributeType = attr_t;
    using ColorType     = NCURSES_COLOR_T;
    using PairType      = NCURSES_PAIRS_T;

public:
    enum class ColorTable : ColorType
    {
        BLACK   = COLOR_BLACK   ,
        RED     = COLOR_RED     ,
        GREEN   = COLOR_GREEN   ,
        YELLOW  = COLOR_YELLOW  ,
        BLUE    = COLOR_BLUE    ,
        MAGENTA = COLOR_MAGENTA ,
        CYAN    = COLOR_CYAN    ,
        WHITE   = COLOR_WHITE   ,
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
    inline NcursesWindow * getStandardWindow() {
        return stdscr;
    }

    NcursesWindow * initialize() {
        return ::initscr();
    }

    int release() {
        return ::endwin();
    }

    template <typename ... Args>
    int print(std::string const & format, Args ... args) {
        return ::printw(format.c_str(), args...);
    }

    template <typename ... Args>
    int movePrint(int x, int y, std::string const & format, Args ... args) {
        return ::mvprintw(y, x, format.c_str(), args...);
    }

    int getChar() {
        return ::getch();
    }

    int getString(char * result, std::size_t buffer_size) {
        return ::wgetnstr(getStandardWindow(), result, buffer_size);
    }

    int clear() {
        return ::clear();
    }

// Window new/del methods.
public:
    NcursesWindow * createWindow(int x, int y, int width, int height) {
        return ::newwin(height, width, y, x);
    }

    int destroyWindow(NcursesWindow * window) {
        return ::delwin(window);
    }

// Update methods.
public:
    int refresh() {
        return ::refresh();
    }

    int refresh(NcursesWindow * window) {
        return ::wrefresh(window);
    }

// Window style.
public:
    int setBox(NcursesWindow * window, CharType vertical, CharType horizontal) {
        return ::box(window, vertical, horizontal);
    }

    int setBox(NcursesWindow * window, char vertical, char horizontal) {
        return setBox(window
                    , static_cast<CharType>(vertical)
                    , static_cast<CharType>(horizontal));
    }

    /**
     * @param window [in] Window pointer.
     * @param     ls [in] left side.
     * @param     rs [in] right side.
     * @param     ts [in] top side.
     * @param     bs [in] bottom side.
     * @param     tl [in] top left-hand corner.
     * @param     tr [in] top right-hand corner.
     * @param     bl [in] bottom left-hand corner.
     * @param     br [in] bottom right-hand corner.
     */
    template <typename T>
    int setBorder(NcursesWindow * window, T ls, T rs
                                        , T ts, T bs
                                        , T tl, T tr
                                        , T bl, T br) {
        return ::wborder(window, static_cast<CharType>(ls), static_cast<CharType>(rs)
                               , static_cast<CharType>(ts), static_cast<CharType>(bs)
                               , static_cast<CharType>(tl), static_cast<CharType>(tr)
                               , static_cast<CharType>(bl), static_cast<CharType>(br));
    }

public:
    template <typename ... Args>
    int movePrintWithWindow(NcursesWindow * window, int x, int y, std::string const & format, Args ... args) {
        return ::mvwprintw(window, y, x, format.c_str(), args...);
    }

// Color methods.
public:
    int startColorMode() {
        return ::start_color();
    }

    int initColor(ColorType color, ColorType r, ColorType g, ColorType b) {
        return ::init_color(color, r, g, b);
    }

    PairType getColorPair(int number) {
        return COLOR_PAIR(number);
    }

    template <typename T>
    int initPair(PairType pair, T foreground, T background) {
        return ::init_pair(pair
                         , static_cast<ColorType>(foreground)
                         , static_cast<ColorType>(background));
    }

// Terminal methods.
public:
    int setRaw() {
        return ::raw();
    }

    int setCbreak() {
        return ::cbreak();
    }

    int setEcho() {
        return ::echo();
    }

    int setNoEcho() {
        return ::noecho();
    }

// Attribute methods.
public:
    int onAttribute(AttributeType flags) {
        return ::attron(flags);
    }

    int offAttribute(AttributeType flags) {
        return ::attroff(flags);
    }

public:
    int onAttributeWithColorPair(PairType pair) {
        return ::attron(pair);
    }

    int offAttributeWithColorPair(PairType pair) {
        return ::attron(pair);
    }

public:
    int setKeypad(NcursesWindow * window, bool enable_function_key) {
        return ::keypad(window, (enable_function_key ? TRUE : FALSE));
    }

    int setCursor(int flag) {
        return ::curs_set(flag);
    }

// Query methods.
public:
    int getCursorX(NcursesWindow * window) {
        return getcurx(window);
    };
    int getCursorY(NcursesWindow * window) {
        return getcury(window);
    };

    int getBeginningX(NcursesWindow * window) {
        return getbegx(window);
    };
    int getBeginningY(NcursesWindow * window) {
        return getbegy(window);
    };

    int getMaxX(NcursesWindow * window) {
        return getmaxx(window);
    };
    int getMaxY(NcursesWindow * window) {
        return getmaxy(window);
    };

    int getParentRelativeX(NcursesWindow * window) {
        return getparx(window);
    };
    int getParentRelativeY(NcursesWindow * window) {
        return getpary(window);
    };

public:
    inline int getTerminalWidth() {
        return COLS;
    }

    inline int getTerminalHeight() {
        return LINES;
    }
};

} // namespace curses

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CURSES_CONTEXT_HPP__

