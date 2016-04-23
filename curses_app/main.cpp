/**
 * @file   main.cpp
 * @brief  Program entry-point.
 * @author zer0
 * @date   2016-04-23
 */

#include <iostream>
#include <libtbag/curses/Context.hpp>

using namespace libtbag;

int main(int argc, char ** argv)
{
    curses::Context context;
    context.init();
    context.print("Hello, world!\n");
    context.refresh();
    context.getChar();
    context.release();

    return 0;
}

