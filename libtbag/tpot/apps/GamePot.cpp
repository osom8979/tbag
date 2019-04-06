/**
 * @file   GamePot.cpp
 * @brief  GamePot class implementation.
 * @author zer0
 * @date   2019-01-31
 */

#include <libtbag/tpot/apps/GamePot.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/game/GameEngine.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tpot {
namespace apps {

GamePot::GamePot()
{
    // EMPTY.
}

GamePot::~GamePot()
{
    // EMPTY.
}

char const * const GamePot::name() TBAG_NOEXCEPT
{
    return "game";
}

char const * const GamePot::remarks() TBAG_NOEXCEPT
{
    return "Play Game";
}

char const * const GamePot::help() TBAG_NOEXCEPT
{
    return "";
}

char const * const GamePot::getName() const TBAG_NOEXCEPT
{
    return name();
}

char const * const GamePot::getRemarks() const TBAG_NOEXCEPT
{
    return remarks();
}

char const * const GamePot::getHelp() const TBAG_NOEXCEPT
{
    return help();
}

int GamePot::onMain()
{
    return libtbag::game::runGame(params().storage);
}

} // namespace apps
} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

