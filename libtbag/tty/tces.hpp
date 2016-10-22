/**
 * @file   tces.hpp
 * @brief  Terminal Control Escape Sequences.
 * @author zer0
 * @date   2016-07-21
 *
 * @remarks
 *  ANSI/VT100 Terminal Control Escape Sequences
 *
 * @see <http://www.termsys.demon.co.uk/vtansi.htm>
 * @see <http://misc.flogisoft.com/bash/tip_colors_and_formatting>
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TTY_TCES_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TTY_TCES_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tces {

// ----------------------------------------------
// Set Attribute Mode: <ESC>[{attr1};...;{attrn}m
// ----------------------------------------------

char const * const DISPLAY_ATTRIBUTE_RESET      = "\x1B[0m";
char const * const DISPLAY_ATTRIBUTE_BRIGHT     = "\x1B[1m";
char const * const DISPLAY_ATTRIBUTE_DIM        = "\x1B[2m";
char const * const DISPLAY_ATTRIBUTE_UNDERSCORE = "\x1B[4m";
char const * const DISPLAY_ATTRIBUTE_BLINK      = "\x1B[5m";
char const * const DISPLAY_ATTRIBUTE_REVERSE    = "\x1B[7m";
char const * const DISPLAY_ATTRIBUTE_HIDDEN     = "\x1B[8m";

// Foreground Colours
char const * const DISPLAY_ATTRIBUTE_FG_BLACK   = "\x1B[30m";
char const * const DISPLAY_ATTRIBUTE_FG_RED     = "\x1B[31m";
char const * const DISPLAY_ATTRIBUTE_FG_GREEN   = "\x1B[32m";
char const * const DISPLAY_ATTRIBUTE_FG_YELLOW  = "\x1B[33m";
char const * const DISPLAY_ATTRIBUTE_FG_BLUE    = "\x1B[34m";
char const * const DISPLAY_ATTRIBUTE_FG_MAGENTA = "\x1B[35m";
char const * const DISPLAY_ATTRIBUTE_FG_CYAN    = "\x1B[36m";
char const * const DISPLAY_ATTRIBUTE_FG_WHITE   = "\x1B[37m";

// Background Colours
char const * const DISPLAY_ATTRIBUTE_BG_BLACK   = "\x1B[40m";
char const * const DISPLAY_ATTRIBUTE_BG_RED     = "\x1B[41m";
char const * const DISPLAY_ATTRIBUTE_BG_GREEN   = "\x1B[42m";
char const * const DISPLAY_ATTRIBUTE_BG_YELLOW  = "\x1B[43m";
char const * const DISPLAY_ATTRIBUTE_BG_BLUE    = "\x1B[44m";
char const * const DISPLAY_ATTRIBUTE_BG_MAGENTA = "\x1B[45m";
char const * const DISPLAY_ATTRIBUTE_BG_CYAN    = "\x1B[46m";
char const * const DISPLAY_ATTRIBUTE_BG_WHITE   = "\x1B[47m";

// ------------------
// WIDE CHAR VERSION.
// ------------------

wchar_t const * const DISPLAY_ATTRIBUTE_RESET_WIDE      = L"\x1B[0m";
wchar_t const * const DISPLAY_ATTRIBUTE_BRIGHT_WIDE     = L"\x1B[1m";
wchar_t const * const DISPLAY_ATTRIBUTE_DIM_WIDE        = L"\x1B[2m";
wchar_t const * const DISPLAY_ATTRIBUTE_UNDERSCORE_WIDE = L"\x1B[4m";
wchar_t const * const DISPLAY_ATTRIBUTE_BLINK_WIDE      = L"\x1B[5m";
wchar_t const * const DISPLAY_ATTRIBUTE_REVERSE_WIDE    = L"\x1B[7m";
wchar_t const * const DISPLAY_ATTRIBUTE_HIDDEN_WIDE     = L"\x1B[8m";

// Foreground Colours
wchar_t const * const DISPLAY_ATTRIBUTE_FG_BLACK_WIDE   = L"\x1B[30m";
wchar_t const * const DISPLAY_ATTRIBUTE_FG_RED_WIDE     = L"\x1B[31m";
wchar_t const * const DISPLAY_ATTRIBUTE_FG_GREEN_WIDE   = L"\x1B[32m";
wchar_t const * const DISPLAY_ATTRIBUTE_FG_YELLOW_WIDE  = L"\x1B[33m";
wchar_t const * const DISPLAY_ATTRIBUTE_FG_BLUE_WIDE    = L"\x1B[34m";
wchar_t const * const DISPLAY_ATTRIBUTE_FG_MAGENTA_WIDE = L"\x1B[35m";
wchar_t const * const DISPLAY_ATTRIBUTE_FG_CYAN_WIDE    = L"\x1B[36m";
wchar_t const * const DISPLAY_ATTRIBUTE_FG_WHITE_WIDE   = L"\x1B[37m";

// Background Colours
wchar_t const * const DISPLAY_ATTRIBUTE_BG_BLACK_WIDE   = L"\x1B[40m";
wchar_t const * const DISPLAY_ATTRIBUTE_BG_RED_WIDE     = L"\x1B[41m";
wchar_t const * const DISPLAY_ATTRIBUTE_BG_GREEN_WIDE   = L"\x1B[42m";
wchar_t const * const DISPLAY_ATTRIBUTE_BG_YELLOW_WIDE  = L"\x1B[43m";
wchar_t const * const DISPLAY_ATTRIBUTE_BG_BLUE_WIDE    = L"\x1B[44m";
wchar_t const * const DISPLAY_ATTRIBUTE_BG_MAGENTA_WIDE = L"\x1B[45m";
wchar_t const * const DISPLAY_ATTRIBUTE_BG_CYAN_WIDE    = L"\x1B[46m";
wchar_t const * const DISPLAY_ATTRIBUTE_BG_WHITE_WIDE   = L"\x1B[47m";

} // namespace tces

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TTY_TCES_HPP__

