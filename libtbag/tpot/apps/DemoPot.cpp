/**
 * @file   DemoPot.cpp
 * @brief  DemoPot class implementation.
 * @author zer0
 * @date   2019-04-06
 */

#include <libtbag/tpot/apps/DemoPot.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/log/Log.hpp>

#include <libtbag/game/Game.hpp>

#include <cstdlib>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tpot {
namespace apps {

struct DemoGame : public libtbag::game::GameEngine
{
    GuiRect rect1;

    DemoGame(Storage const & storage) : libtbag::game::GameEngine(storage)
    {
        rect1.setPosition(10, 10);
        rect1.setSize(20, 20);
        rect1.setFillColor(255, 0, 0, 255);
        rect1.setOutlineColor(0, 255, 0, 255);
        rect1.setOutlineThickness(1);
        rect1.setOrigin(10, 10);
        rect1.move(30, 30);
        rect1.rotate(45);
        rect1.scale(2, 2);
    }

    virtual ~DemoGame()
    {
        // EMPTY.
    }

    virtual void onKeyPressed(GameKey code, bool alt, bool control, bool shift, bool system) override
    {
        if (code == GameKey::Escape) {
            closeGame();
        }
    }

    virtual void onDraw(GameState & state) override
    {
        draw(rect1);
    }
};

// ----------------------
// DemoPot implementation
// ----------------------

DemoPot::DemoPot()
{
    // EMPTY.
}

DemoPot::~DemoPot()
{
    // EMPTY.
}

char const * const DemoPot::name() TBAG_NOEXCEPT
{
    return "demo";
}

char const * const DemoPot::remarks() TBAG_NOEXCEPT
{
    return "Play Demo Game";
}

char const * const DemoPot::help() TBAG_NOEXCEPT
{
    return "";
}

char const * const DemoPot::getName() const TBAG_NOEXCEPT
{
    return name();
}

char const * const DemoPot::getRemarks() const TBAG_NOEXCEPT
{
    return remarks();
}

char const * const DemoPot::getHelp() const TBAG_NOEXCEPT
{
    return help();
}

int DemoPot::onMain()
{
    return DemoGame(storage()).run();
}

} // namespace apps
} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

