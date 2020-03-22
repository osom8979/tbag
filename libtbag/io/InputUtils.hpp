/**
 * @file   InputUtils.hpp
 * @brief  InputUtils class prototype.
 * @author zer0
 * @date   2020-03-22
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_IO_INPUTUTILS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_IO_INPUTUTILS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace io {

TBAG_CONSTEXPR int const INPUT_BUFFER_SIZE = 2048;
TBAG_CONSTEXPR char const * const DEFAULT_READ_INPUT_MESSAGE = "Enter EOF(CTRL+D) to exit the input mode.";

/**
 * The string is received from standard input(<code>stdin</code>) until EOF(<code>CTRL+D</code>) is received.
 */
TBAG_API std::string readInput(int buffer_size = INPUT_BUFFER_SIZE);

} // namespace io

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_IO_INPUTUTILS_HPP__

