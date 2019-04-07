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
    libtbag::game::gui::GuiFont font;

    GuiRect rect1;
    GuiText text1;
    GuiText text2;
    GuiCircle circle1;

    DemoGame(Storage const & storage) : libtbag::game::GameEngine(storage)
    {
        if (!font.loadNanumGothicCodingBold()) {
            tDLogE("DemoGame::DemoGame() font2 load error.");
        }

        rect1.setPosition(10, 10);
        rect1.setSize(20, 20);
        rect1.setFillColor(255, 0, 0, 255);
        rect1.setOutlineColor(0, 255, 0, 255);
        rect1.setOutlineThickness(1);
        rect1.move(30, 30);
        rect1.rotate(45);
        rect1.scale(2, 2);

        text1.setFont(font);
        text1.setString("Hello, World");
        text1.setPosition(0, 0);
        text1.setCharacterSize(24);
        text1.setFillColor(0, 0, 255, 255);
        text1.setStyle(GuiText::STYLE_REGULAR|GuiText::STYLE_UNDERLINED);

        text2.setFont(font);
        text2.setString("Hello, World");
        text2.setPosition(0, 30);
        text2.setCharacterSize(24);
        text2.setFillColor(0, 255, 0, 255);
        text2.setStyle(GuiText::STYLE_BOLD|GuiText::STYLE_STRIKETHROUGH);

        circle1.setPosition(100, 30);
        circle1.setFillColor(255, 255, 0, 255);
        circle1.setOutlineColor(255, 0, 255, 255);
        circle1.setOutlineThickness(1);
        circle1.setRadius(30);
        circle1.setOrigin(0.5, 0.5);
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
        draw(circle1);
        draw(text1);
        draw(text2);
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

