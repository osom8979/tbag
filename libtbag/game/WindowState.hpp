/**
 * @file   WindowState.hpp
 * @brief  WindowState class prototype.
 * @author zer0
 * @date   2019-03-24
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GAME_WINDOWSTATE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GAME_WINDOWSTATE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

#include <cstdint>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace game {

/**
 * WindowState class prototype.
 *
 * @author zer0
 * @date   2019-03-24
 */
struct TBAG_API WindowState
{
    std::int64_t delta = 0;

    WindowState();
    ~WindowState();
};

} // namespace game

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GAME_WINDOWSTATE_HPP__

