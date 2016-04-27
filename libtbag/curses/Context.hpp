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
#include <vector>

#include <ncurses/ncurses.h>

#define NCURSES_EXTENSION

#if defined(NCURSES_EXTENSION) && defined(A_ITALIC)
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

public:
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

public:
    int getChar(NcursesWindow * window) {
        return ::wgetch(window);
    }

    int getChar() {
        return getChar(getStandardWindow());
    }

public:
    int getString(NcursesWindow * window, char * result, std::size_t buffer_size) {
        return ::wgetnstr(window, result, buffer_size);
    }

    int getString(char * result, std::size_t buffer_size) {
        return getString(getStandardWindow(), result, buffer_size);
    }

public:
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
    int update(NcursesWindow * window) {
        return ::wrefresh(window);
    }

    int update() {
        return update(getStandardWindow());
    }

// Window style.
public:
    int setBox(NcursesWindow * window, CharType vertical, CharType horizontal) {
        return ::box(window, vertical, horizontal);
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
    int setBorder(NcursesWindow * window, CharType ls, CharType rs
                                        , CharType ts, CharType bs
                                        , CharType tl, CharType tr
                                        , CharType bl, CharType br) {
        return ::wborder(window, ls, rs, ts, bs, tl, tr, bl, br);
    }

// Color methods.
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

    int startColorMode() {
        return ::start_color();
    }

    int initColor(ColorType color, ColorType r, ColorType g, ColorType b) {
        return ::init_color(color, r, g, b);
    }

public:
    PairType getColorPair(int number) {
        return COLOR_PAIR(number);
    }

    int initPair(PairType  pair
               , ColorType foreground
               , ColorType background) {
        return ::init_pair(pair, foreground, background);
    }

    int initPair(PairType   pair
               , ColorTable foreground
               , ColorTable background) {
        return initPair(pair, static_cast<ColorType>(foreground)
                            , static_cast<ColorType>(background));
    }

    int initPairWithPairNumber(int       pair_number
                             , ColorType foreground
                             , ColorType background) {
        return initPair(getColorPair(pair_number), foreground, background);
    }

    int initPairWithPairNumber(int        pair_number
                             , ColorTable foreground
                             , ColorTable background) {
        return initPair(getColorPair(pair_number), foreground, background);
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

    int setNoecho() {
        return ::noecho();
    }

public:
    int setKeypad(NcursesWindow * window, bool enable_function_key) {
        return ::keypad(window, (enable_function_key ? TRUE : FALSE));
    }

    int setCursor(int flag) {
        return ::curs_set(flag);
    }

// Attribute methods.
public:
    enum class AttributeTable : CharType
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

    CharType getCharType(char c, std::vector<AttributeTable> const & attributes) {
        CharType result = static_cast<CharType>(c);
        for (auto cursor : attributes) {
            result |= static_cast<CharType>(cursor);
        }
        return result;
    }

// Attribute ON methods.
public:
    int onAttribute(NcursesWindow * window, AttributeType flags) {
        return ::wattron(window, flags);
    }

    int onAttribute(AttributeType flags) {
        return onAttribute(getStandardWindow(), flags);
    }

    int onAttribute(NcursesWindow * window, PairType pair) {
        return ::wattron(window, pair);
    }

    int onAttribute(PairType pair) {
        return onAttribute(getStandardWindow(), pair);
    }

// Attribute OFF methods.
public:
    int offAttribute(NcursesWindow * window, AttributeType flags) {
        return ::wattroff(window, flags);
    }

    int offAttribute(AttributeType flags) {
        return offAttribute(getStandardWindow(), flags);
    }

    int offAttribute(NcursesWindow * window, PairType pair) {
        return ::wattroff(window, pair);
    }

    int offAttribute(PairType pair) {
        return offAttribute(getStandardWindow(), pair);
    }

public:
    inline int getCursorX(NcursesWindow * window) {
        return getcurx(window);
    };

    inline int getCursorY(NcursesWindow * window) {
        return getcury(window);
    };

public:
    inline int getBeginningX(NcursesWindow * window) {
        return getbegx(window);
    };

    inline int getBeginningY(NcursesWindow * window) {
        return getbegy(window);
    };

public:
    inline int getMaxX(NcursesWindow * window) {
        return getmaxx(window);
    };

    inline int getMaxY(NcursesWindow * window) {
        return getmaxy(window);
    };

public:
    inline int getParentRelativeX(NcursesWindow * window) {
        return getparx(window);
    };

    inline int getParentRelativeY(NcursesWindow * window) {
        return getpary(window);
    };

public:
    inline int getTerminalWidth() {
        return COLS;
    }

    inline int getTerminalHeight() {
        return LINES;
    }


public:
    int move(NcursesWindow * window, int x, int y) {
        return ::wmove(window, y, x);
    }

    int move(int x, int y) {
        return move(getStandardWindow(), x, y);
    }

public:
    /**
     * Print single character & move next cursor.
     */
    int addChar(NcursesWindow * window, char c, CharType flags = 0) {
        return ::waddch(window, static_cast<CharType>(c) | flags);
    }

    /**
     * Print single character & move next cursor.
     */
    int addChar(char c, CharType flags = 0) {
        return addChar(getStandardWindow(), c, flags);
    }

public:
    template <typename ... Args>
    int print(NcursesWindow * window, std::string const & format, Args ... args) {
        return ::wprintw(window, format.c_str(), args...);
    }

    template <typename ... Args>
    int print(std::string const & format, Args ... args) {
        return print(getStandardWindow(), format, args...);
    }

public:
    template <typename ... Args>
    int movePrint(NcursesWindow * window, int x, int y, std::string const & format, Args ... args) {
        return ::mvwprintw(window, y, x, format.c_str(), args...);
    }

    template <typename ... Args>
    int movePrint(int x, int y, std::string const & format, Args ... args) {
        return movePrint(getStandardWindow(), x, y, format, args...);
    }
};

} // namespace curses

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CURSES_CONTEXT_HPP__

