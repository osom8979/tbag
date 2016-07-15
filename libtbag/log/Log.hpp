/**
 * @file   Log.hpp
 * @brief  Log class prototype.
 * @author zer0
 * @date   2016-04-04
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOG_LOG_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOG_LOG_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/pattern/Singleton.hpp>

#include <atomic>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log {

//constexpr char const * const DEFAULT_LOGGER_OF_CONSOLE = "_C";
//constexpr char const * const DEFAULT_LOGGER_OF_FILE = "_F";
//constexpr char const * const DEFAULT_LOGGER_FORMAT = "%L [%Y-%m-%dT%H:%M:%S.%e] [%t] %v";

constexpr bool isAsynchronousLogging() noexcept
{
    return true;
}

constexpr bool isMultithreadLogging() noexcept
{
    return false;
}

} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_LOG_HPP__

