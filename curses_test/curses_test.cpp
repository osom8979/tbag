/**
 * @file   curses_test.cpp
 * @brief  Program entry-point.
 * @author zer0
 * @date   2016-04-23
 */

#include <libtbag/curses/Application.hpp>

using namespace libtbag;
using namespace libtbag::curses;

void testCurses()
{
    int const BUFFER_SIZE = 256;
    char STR_BUFFER[BUFFER_SIZE] = { 0, };

    using Application = ::libtbag::curses::Application;
    Application app;
    app.clear();

    app.startColorMode();
    app.initPair(1, Application::CYAN , Application::MAGENTA);
    app.initPair(2, Application::GREEN, Application::BLUE);

    app.print("String input: ");
    app.getString(STR_BUFFER, BUFFER_SIZE);
    app.print("Result string: %s\n", STR_BUFFER);

    app.setNoecho();
    app.print("Press any key to continue...\n");
    app.update();
    app.getChar();

    app.setCbreak();
    app.setKeypad(true);
    app.setCursor(0);

    app.move(0, 4);
    app.addChar('!');
    app.print("PRINT.\n");

    int cols  = app.getTerminalWidth();
    int lines = app.getTerminalHeight();
    app.movePrint(0, 5, "Width: %d, Height: %d\n", cols, lines);
    app.update();

    Window win(0, 7, 32, 10);
    win.setBox('|', '-');

    int x1 = win.getCursorX();
    int y1 = win.getCursorY();
    int x2 = win.getBeginningX();
    int y2 = win.getBeginningY();
    int x3 = win.getMaxX();
    int y3 = win.getMaxY();
    int x4 = win.getParentRelativeX();
    int y4 = win.getParentRelativeY();

    win.onColorAttribute(1);
    win.movePrint(2, 2, "Cursor: %d, %d"   , x1, y1);
    win.movePrint(2, 3, "Beginning: %d, %d", x2, y2);
    win.offColorAttribute(1);
    win.onColorAttribute(2);
    win.movePrint(2, 4, "Max: %d, %d"      , x3, y3);
    win.movePrint(2, 5, "Parent: %d, %d"   , x4, y4);
    win.offColorAttribute(2);

    win.move(2, 6);
    win.print("Press any key to continue...");
    win.update();
    win.getChar();

    win.setBorder('1', '2', '3', '4', '5', '6', '7', '8');
    win.update();

    app.move(0, y2 + y3 + 1);
    app.print("Press any key to continue...\n");
    app.update();
    app.getChar();

    app.release();
}

int main(int argc, char ** argv)
{
    testCurses();
    return 0;
}

