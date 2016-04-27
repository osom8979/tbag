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
#include <string>

#include <ncurses/ncurses.h>

#if defined(A_ITALIC)
# define ATTRIBUTE_TABLE_ITALIC  A_ITALIC
#else
# define ATTRIBUTE_TABLE_ITALIC  NCURSES_BITS(1U, 23)
#endif

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

protected:
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
    static inline NcursesWindow * getStandardWindow() {
        return stdscr;
    }

    static NcursesWindow * initialize() {
        return ::initscr();
    }

    static int release() {
        return ::endwin();
    }

public:
    static int getChar(NcursesWindow * window) {
        return ::wgetch(window);
    }

    static int getString(NcursesWindow * window, char * result, std::size_t buffer_size) {
        return ::wgetnstr(window, result, buffer_size);
    }

public:
    static int clear() {
        return ::clear();
    }

// Window new/del methods.
public:
    static NcursesWindow * createWindow(int x, int y, int width, int height) {
        return ::newwin(height, width, y, x);
    }

    static int destroyWindow(NcursesWindow * window) {
        return ::delwin(window);
    }

// Update methods.
public:
    static int update(NcursesWindow * window) {
        return ::wrefresh(window);
    }

// Terminal methods.
public:
    static int setRaw() {
        return ::raw();
    }

    static int setCbreak() {
        return ::cbreak();
    }

    static int setEcho() {
        return ::echo();
    }

    static int setNoecho() {
        return ::noecho();
    }

    static int setCursor(int flag) {
        return ::curs_set(flag);
    }

public:
    static int setKeypad(NcursesWindow * window, bool enable_function_key) {
        return ::keypad(window, (enable_function_key ? TRUE : FALSE));
    }

// Cursor methods.
public:
    static int move(NcursesWindow * window, int x, int y) {
        return ::wmove(window, y, x);
    }

// Output methods.
public:
    /** Print single character & move next cursor. */
    static int addChar(NcursesWindow * window, char c, CharType flags = 0) {
        return ::waddch(window, static_cast<CharType>(c) | flags);
    }

public:
    template <typename ... Args>
    static int print(NcursesWindow * window, std::string const & format, Args ... args) {
        return ::wprintw(window, format.c_str(), args...);
    }

public:
    template <typename ... Args>
    static int movePrint(NcursesWindow * window, int x, int y, std::string const & format, Args ... args) {
        return ::mvwprintw(window, y, x, format.c_str(), args...);
    }

// Attribute methods.
public:
    enum AttributeTable
    {
        NORMAL      = A_NORMAL,
        ATTRIBUTES  = A_ATTRIBUTES,
        CHARTEXT    = A_CHARTEXT,
        COLOR       = A_COLOR,
        STANDOUT    = A_STANDOUT,
        UNDERLINE   = A_UNDERLINE,
        REVERSE     = A_REVERSE,
        BLINK       = A_BLINK,
        DIM         = A_DIM,
        BOLD        = A_BOLD,
        ALTCHARSET  = A_ALTCHARSET,
        INVIS       = A_INVIS,
        PROTECT     = A_PROTECT,
        HORIZONTAL  = A_HORIZONTAL,
        LEFT        = A_LEFT,
        LOW         = A_LOW,
        RIGHT       = A_RIGHT,
        TOP         = A_TOP,
        VERTICAL    = A_VERTICAL,
        ITALIC      = ATTRIBUTE_TABLE_ITALIC,
    };

    enum ColorTable
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

// Attribute ON methods.
public:
    static int onAttribute(NcursesWindow * window, AttributeType flags) {
        return ::wattron(window, flags);
    }

    static int onAttribute(NcursesWindow * window, PairType pair) {
        return ::wattron(window, pair);
    }

// Attribute OFF methods.
public:
    static int offAttribute(NcursesWindow * window, AttributeType flags) {
        return ::wattroff(window, flags);
    }

    static int offAttribute(NcursesWindow * window, PairType pair) {
        return ::wattroff(window, pair);
    }

public:
    static int startColorMode() {
        return ::start_color();
    }

    static bool hasColors() {
        return (::has_colors() == TRUE ? true : false);
    }

public:
    static int initColor(ColorType color, ColorType r, ColorType g, ColorType b) {
        return ::init_color(color, r, g, b);
    }

public:
    static PairType getColorPair(int number) {
        return COLOR_PAIR(number);
    }

    static int initPair(int pair_number, ColorType foreground, ColorType background) {
        return ::init_pair(static_cast<PairType>(pair_number), foreground, background);
    }

public:
    static int onColorAttribute(NcursesWindow * window, int pair_number) {
        return onAttribute(window, getColorPair(pair_number));
    }

    static int offColorAttribute(NcursesWindow * window, int pair_number) {
        return offAttribute(window, getColorPair(pair_number));
    }

// Window style.
public:
    int setBox(NcursesWindow * window, CharType vertical, CharType horizontal) {
        return ::box(window, vertical, horizontal);
    }

    /**
     * @param window [in] NCurses WINDOW.
     * @param     ls [in] left side.
     * @param     rs [in] right side.
     * @param     ts [in] top side.
     * @param     bs [in] bottom side.
     * @param     tl [in] top left-hand corner.
     * @param     tr [in] top right-hand corner.
     * @param     bl [in] bottom left-hand corner.
     * @param     br [in] bottom right-hand corner.
     */
    int setBorder(NcursesWindow * window, CharType ls, CharType rs
                                        , CharType ts, CharType bs
                                        , CharType tl, CharType tr
                                        , CharType bl, CharType br) {
        return ::wborder(window, ls, rs, ts, bs, tl, tr, bl, br);
    }

// Position mehtod.
public:
    static inline int getCursorX(NcursesWindow * window) {
        return getcurx(window);
    }

    static inline int getCursorY(NcursesWindow * window) {
        return getcury(window);
    }

public:
    static inline int getBeginningX(NcursesWindow * window) {
        return getbegx(window);
    }

    static inline int getBeginningY(NcursesWindow * window) {
        return getbegy(window);
    }

public:
    static inline int getMaxX(NcursesWindow * window) {
        return getmaxx(window);
    }

    static inline int getMaxY(NcursesWindow * window) {
        return getmaxy(window);
    }

public:
    static inline int getParentRelativeX(NcursesWindow * window) {
        return getparx(window);
    }

    static inline int getParentRelativeY(NcursesWindow * window) {
        return getpary(window);
    }

public:
    static inline int getTerminalWidth() {
        return COLS;
    }

    static inline int getTerminalHeight() {
        return LINES;
    }
};

} // namespace curses

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CURSES_CONTEXT_HPP__

