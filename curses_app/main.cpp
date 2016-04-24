/**
 * @file   main.cpp
 * @brief  Program entry-point.
 * @author zer0
 * @date   2016-04-23
 */

#include <iostream>
#include <libtbag/curses/Context.hpp>

using namespace libtbag;

void onlyTypeCoverage()
{
    LIBTBAG_CURSES_SCREEN  * v1 = nullptr;
    LIBTBAG_CURSES_WINDOW  * v2 = nullptr;
    LIBTBAG_CURSES_CHAR    * v3 = nullptr;
    LIBTBAG_CURSES_ATTR    * v4 = nullptr;
    LIBTBAG_CURSES_COLOR   * v5 = nullptr;
    LIBTBAG_CURSES_PAIRS   * v6 = nullptr;

    REMOVE_UNUSED_VARIABLE(v1);
    REMOVE_UNUSED_VARIABLE(v2);
    REMOVE_UNUSED_VARIABLE(v3);
    REMOVE_UNUSED_VARIABLE(v4);
    REMOVE_UNUSED_VARIABLE(v5);
    REMOVE_UNUSED_VARIABLE(v6);
}

void testCurses()
{
    int const BUFFER_SIZE = 256;
    char STR_BUFFER[BUFFER_SIZE] = { 0, };
    LIBTBAG_CURSES_CLEAR();
    LIBTBAG_CURSES_INITSCR();
    LIBTBAG_CURSES_MVPRINTW(0, 0, "Col: %d, Lines: %d\n", COLS, LINES);
    LIBTBAG_CURSES_REFRESH();

    LIBTBAG_CURSES_START_COLOR();
    //LIBTBAG_CURSES_INIT_COLOR(c, r, g, b);
    LIBTBAG_CURSES_INIT_PAIR(1, LIBTBAG_CURSES_COLOR_RED, LIBTBAG_CURSES_COLOR_BLUE);
    LIBTBAG_CURSES_INIT_PAIR(2, LIBTBAG_CURSES_COLOR_GREEN, LIBTBAG_CURSES_COLOR_YELLOW);

    int x1 = LIBTBAG_CURSES_GETX(LIBTBAG_CURSES_STDSCR);
    int y1 = LIBTBAG_CURSES_GETY(LIBTBAG_CURSES_STDSCR);
    int x2 = LIBTBAG_CURSES_GETBEGX(LIBTBAG_CURSES_STDSCR);
    int y2 = LIBTBAG_CURSES_GETBEGY(LIBTBAG_CURSES_STDSCR);
    int x3 = LIBTBAG_CURSES_GETMAXX(LIBTBAG_CURSES_STDSCR);
    int y3 = LIBTBAG_CURSES_GETMAXY(LIBTBAG_CURSES_STDSCR);
    int x4 = LIBTBAG_CURSES_GETPARX(LIBTBAG_CURSES_STDSCR);
    int y4 = LIBTBAG_CURSES_GETPARY(LIBTBAG_CURSES_STDSCR);

    LIBTBAG_CURSES_ATTRON(LIBTBAG_CURSES_COLOR_PAIR(1));
    LIBTBAG_CURSES_PRINTW("Cursor: %d, %d\n", x1, y1);
    LIBTBAG_CURSES_PRINTW("Beginning: %d, %d\n", x2, y2);
    LIBTBAG_CURSES_PRINTW("Max: %d, %d\n", x3, y3);
    LIBTBAG_CURSES_PRINTW("Parent: %d, %d\n", x4, y4);
    LIBTBAG_CURSES_ATTROFF(LIBTBAG_CURSES_COLOR_PAIR(1));

    LIBTBAG_CURSES_PRINTW("String test 01: ");
    LIBTBAG_CURSES_GETSTR(STR_BUFFER);
    LIBTBAG_CURSES_PRINTW("String test 02: ");
    LIBTBAG_CURSES_WGETNSTR(LIBTBAG_CURSES_STDSCR, STR_BUFFER, BUFFER_SIZE);

    LIBTBAG_CURSES_WINDOW * win = LIBTBAG_CURSES_NEWWIN(10, 32, 10, 0);
    LIBTBAG_CURSES_BOX(win, '*', '+');
    LIBTBAG_CURSES_WREFRESH(win);

    LIBTBAG_CURSES_PRINTW("Press any key to continue...\n");
    LIBTBAG_CURSES_GETCH(); // WAIT!

    LIBTBAG_CURSES_ATTRON(LIBTBAG_CURSES_COLOR_PAIR(2));
    LIBTBAG_CURSES_GETYX(win, y1, x1);
    LIBTBAG_CURSES_GETBEGYX(win, y2, x2);
    LIBTBAG_CURSES_GETMAXYX(win, y3, x3);
    LIBTBAG_CURSES_GETPARYX(win, y4, x4);
    LIBTBAG_CURSES_MVWPRINTW(win, 2, 2, "Cursor: %d, %d\n", x1, y1);
    LIBTBAG_CURSES_MVWPRINTW(win, 3, 2, "Beginning: %d, %d\n", x2, y2);
    LIBTBAG_CURSES_MVWPRINTW(win, 4, 2, "Max: %d, %d\n", x3, y3);
    LIBTBAG_CURSES_MVWPRINTW(win, 5, 2, "Parent: %d, %d\n", x4, y4);
    LIBTBAG_CURSES_ATTROFF(LIBTBAG_CURSES_COLOR_PAIR(2));

    LIBTBAG_CURSES_WBORDER(win, '1', '2', '3', '4', '5', '6', '7', '8');
    LIBTBAG_CURSES_WREFRESH(win);
    LIBTBAG_CURSES_DELWIN(win);

    // ONLY COVERAGE.
    LIBTBAG_CURSES_RAW();
    LIBTBAG_CURSES_CBREAK();
    LIBTBAG_CURSES_ECHO();
    LIBTBAG_CURSES_NOECHO();
    LIBTBAG_CURSES_KEYPAD(LIBTBAG_CURSES_STDSCR, TRUE);
    LIBTBAG_CURSES_CURS_SET(0);

    LIBTBAG_CURSES_PRINTW("Press any key to continue...\n");
    LIBTBAG_CURSES_GETCH(); // WAIT!
    LIBTBAG_CURSES_ENDWIN();
}

void testContext()
{
    curses::Context c;

    c.clear();
    c.initialize();
    c.startColorMode();
    c.initPair(3, LIBTBAG_CURSES_COLOR_CYAN, LIBTBAG_CURSES_COLOR_MAGENTA);
    c.onAttribute(LIBTBAG_CURSES_COLOR_PAIR(3));
    c.print("Col: %d, Lines: %d\n", LIBTBAG_CURSES_COLS, LIBTBAG_CURSES_LINES);
    c.offAttribute(LIBTBAG_CURSES_COLOR_PAIR(3));
    c.refresh();

    c.print("Press any key to continue...\n");
    c.getChar(); // WAIT!
    c.release();
}

int main(int argc, char ** argv)
{
    onlyTypeCoverage();
    testCurses();
    testContext();

    return 0;
}

