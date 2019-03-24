/**
 * @file   GamePot.cpp
 * @brief  GamePot class implementation.
 * @author zer0
 * @date   2019-01-31
 */

#include <libtbag/tpot/apps/GamePot.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/gui/Window.hpp>

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

char const * const GamePot::getName() const TBAG_NOEXCEPT
{
    return "game";
}

char const * const GamePot::getRemarks() const TBAG_NOEXCEPT
{
    return "Play Game";
}

char const * const GamePot::getHelp() const TBAG_NOEXCEPT
{
    return "";
}

int GamePot::onMain()
{
    return libtbag::gui::runWindow(params().storage);
}

} // namespace apps
} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

