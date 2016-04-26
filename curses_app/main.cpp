/**
 * @file   main.cpp
 * @brief  Program entry-point.
 * @author zer0
 * @date   2016-04-23
 */

#include <iostream>
#include <libtbag/curses/Context.hpp>

using namespace libtbag;

void pressAnyKey(curses::Context & c)
{
    c.print("Press any key to continue...\n");
    c.getChar(); // WAIT!
}

void testCurses()
{
    using Window = ::libtbag::curses::Context::NcursesWindow;
    using Color  = ::libtbag::curses::Context::ColorTable;

    curses::Context c;

    int const BUFFER_SIZE = 256;
    char STR_BUFFER[BUFFER_SIZE] = { 0, };

    c.clear();
    c.initialize();
    Window * main_window = c.getStandardWindow();

    int cols  = c.getTerminalWidth();
    int lines = c.getTerminalHeight();
    c.movePrint(0, 0, "Width: %d, Height: %d\n", cols, lines);
    c.refresh();

    c.startColorMode();
    c.initPair(1, Color::CYAN , Color::MAGENTA);
    c.initPair(2, Color::GREEN, Color::BLUE);

    int x1 = c.getCursorX(main_window);
    int y1 = c.getCursorY(main_window);
    int x2 = c.getBeginningX(main_window);
    int y2 = c.getBeginningY(main_window);
    int x3 = c.getMaxX(main_window);
    int y3 = c.getMaxY(main_window);
    int x4 = c.getParentRelativeX(main_window);
    int y4 = c.getParentRelativeY(main_window);

    c.onAttributeWithColorPair(c.getColorPair(1));
    c.print("Cursor: %d, %d\n", x1, y1);
    c.print("Beginning: %d, %d\n", x2, y2);
    c.print("Max: %d, %d\n", x3, y3);
    c.print("Parent: %d, %d\n", x4, y4);
    c.offAttributeWithColorPair(c.getColorPair(1));

    c.print("String input: ");
    c.getString(STR_BUFFER, BUFFER_SIZE);

    Window * win = c.createWindow(0, 10, 32, 10);
    c.setBox(win, '*', '+');
    c.refresh(win);

    pressAnyKey(c);

    c.onAttributeWithColorPair(c.getColorPair(2));
    x1 = c.getCursorX(win);
    y1 = c.getCursorY(win);
    x2 = c.getBeginningX(win);
    y2 = c.getBeginningY(win);
    x3 = c.getMaxX(win);
    y3 = c.getMaxY(win);
    x4 = c.getParentRelativeX(win);
    y4 = c.getParentRelativeY(win);
    c.movePrintWithWindow(win, 2, 2, "Cursor: %d, %d\n", x1, y1);
    c.movePrintWithWindow(win, 2, 3, "Beginning: %d, %d\n", x2, y2);
    c.movePrintWithWindow(win, 2, 4, "Max: %d, %d\n", x3, y3);
    c.movePrintWithWindow(win, 2, 5, "Parent: %d, %d\n", x4, y4);
    c.offAttributeWithColorPair(c.getColorPair(2));

    c.setBorder(win, '1', '2', '3', '4', '5', '6', '7', '8');
    c.refresh(win);
    c.destroyWindow(win);

    // ONLY COVERAGE.
    c.setRaw();
    c.setCbreak();
    c.setEcho();
    c.setNoEcho();
    c.setKeypad(main_window, true);
    c.setCursor(0);

    pressAnyKey(c);
    c.release();
}

int main(int argc, char ** argv)
{
    testCurses();
    return 0;
}

