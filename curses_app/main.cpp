/**
 * @file   main.cpp
 * @brief  Program entry-point.
 * @author zer0
 * @date   2016-04-23
 */

#include <iostream>
#include <libtbag/curses/Context.hpp>
#include <libtbag/curses/Window.hpp>

using namespace libtbag;
using namespace libtbag::curses;

void testCurses()
{
    using Color     = Context::ColorTable;
    using Attribute = Context::AttributeTable;

    int const BUFFER_SIZE = 256;
    char STR_BUFFER[BUFFER_SIZE] = { 0, };

    Context c;
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

    Window win(0, 15, 32, 10);
    win.setBox('*', '+');
    int x1 = win.getCursorX();
    int y1 = win.getCursorY();
    int x2 = win.getBeginningX();
    int y2 = win.getBeginningY();
    int x3 = win.getMaxX();
    int y3 = win.getMaxY();
    int x4 = win.getParentRelativeX();
    int y4 = win.getParentRelativeY();
    win.onAttribute(c.getColorPair(1));
    win.movePrint(2, 2, "Cursor: %d, %d", x1, y1);
    win.movePrint(2, 3, "Beginning: %d, %d", x2, y2);
    win.movePrint(2, 4, "Max: %d, %d", x3, y3);
    win.movePrint(2, 5, "Parent: %d, %d", x4, y4);
    win.offAttribute(c.getColorPair(1));

    win.move(2, 6);
    win.print("Press any key to continue...");
    win.update();
    win.getChar(); // WAIT!

    win.setBorder('1', '2', '3', '4', '5', '6', '7', '8');
    win.update();

    c.print("Press any key to continue...\n");
    c.update();
    c.getChar(); // WAIT!

    c.release();
}

int main(int argc, char ** argv)
{
    testCurses();
    return 0;
}

