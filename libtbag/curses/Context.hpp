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
    inline int getCursorX(NcursesWindow * window) {
        return getcurx(window);
    };
    inline int getCursorY(NcursesWindow * window) {
        return getcury(window);
    };

    inline int getBeginningX(NcursesWindow * window) {
        return getbegx(window);
    };
    inline int getBeginningY(NcursesWindow * window) {
        return getbegy(window);
    };

    inline int getMaxX(NcursesWindow * window) {
        return getmaxx(window);
    };
    inline int getMaxY(NcursesWindow * window) {
        return getmaxy(window);
    };

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
    /**
     * Print single character & move next cursor.
     */
    int addChar(NcursesWindow * window, CharType c) {
        return ::waddch(window, c);
    }

    /**
     * Print single character & move next cursor.
     */
    int addChar(CharType c) {
        return addChar(getStandardWindow(), c);
    }
};

//#define addchnstr(str,n)	waddchnstr(stdscr,(str),(n))
//#define addchstr(str)		waddchstr(stdscr,(str))
//#define addnstr(str,n)		waddnstr(stdscr,(str),(n))
//#define addstr(str)		waddnstr(stdscr,(str),-1)
//#define attroff(at)		wattroff(stdscr,(at))
//#define attron(at)		wattron(stdscr,(at))
//#define attrset(at)		wattrset(stdscr,(at))
//#define attr_get(ap,cp,o)	wattr_get(stdscr,(ap),(cp),(o))
//#define attr_off(a,o)		wattr_off(stdscr,(a),(o))
//#define attr_on(a,o)		wattr_on(stdscr,(a),(o))
//#define attr_set(a,c,o)		wattr_set(stdscr,(a),(c),(o))
//#define bkgd(ch)		wbkgd(stdscr,(ch))
//#define bkgdset(ch)		wbkgdset(stdscr,(ch))
//#define chgat(n,a,c,o)		wchgat(stdscr,(n),(a),(c),(o))
//#define clear()			wclear(stdscr)
//#define clrtobot()		wclrtobot(stdscr)
//#define clrtoeol()		wclrtoeol(stdscr)
//#define color_set(c,o)		wcolor_set(stdscr,(c),(o))
//#define delch()			wdelch(stdscr)
//#define deleteln()		winsdelln(stdscr,-1)
//#define echochar(c)		wechochar(stdscr,(c))
//#define erase()			werase(stdscr)
//#define getch()			wgetch(stdscr)
//#define getstr(str)		wgetstr(stdscr,(str))
//#define inch()			winch(stdscr)
//#define inchnstr(s,n)		winchnstr(stdscr,(s),(n))
//#define inchstr(s)		winchstr(stdscr,(s))
//#define innstr(s,n)		winnstr(stdscr,(s),(n))
//#define insch(c)		winsch(stdscr,(c))
//#define insdelln(n)		winsdelln(stdscr,(n))
//#define insertln()		winsdelln(stdscr,1)
//#define insnstr(s,n)		winsnstr(stdscr,(s),(n))
//#define insstr(s)		winsstr(stdscr,(s))
//#define instr(s)		winstr(stdscr,(s))
//#define move(y,x)		wmove(stdscr,(y),(x))
//#define refresh()		wrefresh(stdscr)
//#define scrl(n)			wscrl(stdscr,(n))
//#define setscrreg(t,b)		wsetscrreg(stdscr,(t),(b))
//#define standend()		wstandend(stdscr)
//#define standout()		wstandout(stdscr)
//#define timeout(delay)		wtimeout(stdscr,(delay))
//#define wdeleteln(win)		winsdelln(win,-1)
//#define winsertln(win)		winsdelln(win,1)

} // namespace curses

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CURSES_CONTEXT_HPP__

