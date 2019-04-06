/**
 * @file   GameEngine.hpp
 * @brief  GameEngine class prototype.
 * @author zer0
 * @date   2019-01-31
 * @date   2019-02-13 (Rename: Window -> SfWindow)
 * @date   2019-02-16 (Rename: SfWindow -> RenderWindow)
 * @date   2019-03-23 (Rename: RenderWindow -> Window)
 * @date   2019-04-06 (Rename: Window -> GameEngine)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GAME_GAMEENGINE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GAME_GAMEENGINE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/game/GameCommon.hpp>
#include <libtbag/res/Storage.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace game {

TBAG_CONSTEXPR int const GAME_EXIT_CODE_EXIT_SUCCESS = 1;
TBAG_CONSTEXPR int const GAME_EXIT_CODE_EXIT_FAILURE = 2;
TBAG_CONSTEXPR int const GAME_EXIT_CODE_RESTART = 3;

TBAG_API int runGame(libtbag::res::Storage & storage, GameParams const & params);
TBAG_API int runGame(libtbag::res::Storage & storage);
TBAG_API int runGame();

} // namespace game

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GAME_GAMEENGINE_HPP__

