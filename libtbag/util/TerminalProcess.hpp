/**
 * @file   TerminalProcess.hpp
 * @brief  TerminalProcess class prototype.
 * @author zer0
 * @date   2020-02-13
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UTIL_TERMINALPROCESS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UTIL_TERMINALPROCESS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

#include <iostream>
#include <atomic>
#include <chrono>
#include <thread>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace util {

/**
 * TerminalProcess class prototype.
 *
 * @author zer0
 * @date   2020-02-13
 */
class TerminalProcess : private Noncopyable
{
    TBAG_CONSTEXPR static char const BACKSPACE_CHAR = '\b';
    TBAG_CONSTEXPR static char const PROGRESS_CHAR[] = "*|/-\\|/-\\";
    TBAG_CONSTEXPR static int const PROGRESS_CHAR_SIZE = sizeof(PROGRESS_CHAR) / sizeof(PROGRESS_CHAR[0]) - 1;
    TBAG_CONSTEXPR static int const TERMINAL_UPDATE_MILLISECONDS = 1000;

private:
    std::atomic_bool _exit_flag;

public:
    TerminalProcess() : _exit_flag(false)
    {
        // EMPTY.
    }

    ~TerminalProcess()
    {
        // EMPTY.
    }

public:
    void exit()
    {
        _exit_flag = true;
    }

public:
    void run(int tick_ms = TERMINAL_UPDATE_MILLISECONDS) const
    {
        auto const tick = std::chrono::milliseconds(tick_ms);
        std::size_t loop_count = 0u;
        std::cout << "Now running ... " << PROGRESS_CHAR[0];
        while (!exit_flag) {
            std::cout << BACKSPACE_CHAR << PROGRESS_CHAR[loop_count%PROGRESS_CHAR_SIZE];
            std::cout.flush();
            ++loop_count;
            std::this_thread::sleep_for(tick);
        }
    }
};

} // namespace util

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UTIL_TERMINALPROCESS_HPP__

