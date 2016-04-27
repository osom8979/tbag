/**
 * @file   main.cpp
 * @brief  Program entry-point.
 * @author zer0
 * @date   2016-04-23
 */

#include <iostream>
#include <libtbag/curses/Context.hpp>

using namespace libtbag;

void testCurses()
{
    using Window    = ::libtbag::curses::Context::NcursesWindow;
    using Color     = ::libtbag::curses::Context::ColorTable;
    using Attribute = ::libtbag::curses::Context::AttributeTable;

    int const BUFFER_SIZE = 256;
    char STR_BUFFER[BUFFER_SIZE] = { 0, };

    ::libtbag::curses::Context c;
    c.initialize();
    c.clear();

    c.startColorMode();
    c.initPair(1, Color::CYAN , Color::MAGENTA);
    c.initPair(2, Color::GREEN, Color::BLUE);

    c.print("String input: ");
    c.getString(STR_BUFFER, BUFFER_SIZE);
    c.print("Result string: %s\n", STR_BUFFER);

    c.setNoecho();
    c.print("Press any key to continue...\n");
    c.update();
    c.getChar(); // WAIT!

    // Only coverage.
    //c.setEcho();
    c.setCbreak();
    //c.setRaw();
    c.setKeypad(c.getStandardWindow(), true);
    c.setCursor(0);

    c.move(0, 5);
    c.addChar('a');
    c.addChar(c.getCharType('A', { Attribute::BOLD, Attribute::BLINK }));
    c.print("PRINT.\n");

    int cols  = c.getTerminalWidth();
    int lines = c.getTerminalHeight();
    c.movePrint(0, 6, "Width: %d, Height: %d\n", cols, lines);
    c.update();

    Window * win = c.createWindow(0, 15, 32, 10);
    c.setBox(win, '*', '+');
    int x1 = c.getCursorX(win);
    int y1 = c.getCursorY(win);
    int x2 = c.getBeginningX(win);
    int y2 = c.getBeginningY(win);
    int x3 = c.getMaxX(win);
    int y3 = c.getMaxY(win);
    int x4 = c.getParentRelativeX(win);
    int y4 = c.getParentRelativeY(win);
    c.onAttribute(win, c.getColorPair(1));
    c.movePrint(win, 2, 2, "Cursor: %d, %d", x1, y1);
    c.movePrint(win, 2, 3, "Beginning: %d, %d", x2, y2);
    c.movePrint(win, 2, 4, "Max: %d, %d", x3, y3);
    c.movePrint(win, 2, 5, "Parent: %d, %d", x4, y4);
    c.offAttribute(win, c.getColorPair(1));
    c.update(win);

    c.print("Press any key to continue...\n");
    c.update();
    c.getChar(); // WAIT!

    c.setBorder(win, '1', '2', '3', '4', '5', '6', '7', '8');
    c.update(win);

    c.print("Press any key to continue...\n");
    c.update();
    c.getChar(); // WAIT!

    c.destroyWindow(win);
    c.release();
}

int main(int argc, char ** argv)
{
    testCurses();
    return 0;
}

