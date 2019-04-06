/**
 * @file   GameEngine.cpp
 * @brief  GameEngine class implementation.
 * @author zer0
 * @date   2019-01-31
 * @date   2019-02-13 (Rename: Window -> SfWindow)
 * @date   2019-02-16 (Rename: SfWindow -> RenderWindow)
 * @date   2019-03-23 (Rename: RenderWindow -> Window)
 * @date   2019-04-06 (Rename: Window -> GameEngine)
 */

#include <libtbag/game/GameEngine.hpp>
#include <libtbag/game/LuaGameInterface.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/res/Storage.hpp>
#include <libtbag/geometry/GeometryTypes.hpp>
#include <libtbag/graphic/Color.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/Type.hpp>

#include <imgui.h>
#include <imgui-SFML.h>

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <swoosh.hpp>

#include <cstdlib>
#include <cassert>

#include <iostream>
#include <functional>
#include <algorithm>
#include <utility>
#include <string>
#include <vector>
#include <tuple>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace game {

using Activity   = swoosh::Activity;
using Activities = swoosh::ActivityController;

struct GameScene : public Activity
{
    GameScene(Activities * controller) : Activity(controller)
    {
        // EMPTY.
    }

    virtual ~GameScene()
    {
        // EMPTY.
    }

    virtual void onStart() override
    {
    }

    virtual void onUpdate(double elapsed) override
    {
    }

    virtual void onLeave() override
    {
    }

    virtual void onExit() override
    {
    }

    virtual void onEnter() override
    {
    }

    virtual void onResume() override
    {
    }

    virtual void onDraw(sf::RenderTexture & surface) override
    {
    }

    virtual void onEnd() override
    {
    }
};

/**
 * GameEngine backend implementation.
 *
 * @author zer0
 * @date   2019-03-23
 * @date   2019-04-06 (Rename: Window -> GameEngine)
 */
struct GameEngine : public LuaGameInterface, public libtbag::geometry::GeometryTypes
{
    using Channel = libtbag::graphic::Channel;
    using Rgb24   = libtbag::graphic::Rgb24;
    using Rgb32   = libtbag::graphic::Rgb32;

    using Storage    = libtbag::res::Storage;
    using Activities = swoosh::ActivityController;

    TBAG_CONSTEXPR static Channel const CHANNEL_MAX  = libtbag::graphic::channel_max();
    TBAG_CONSTEXPR static Channel const CHANNEL_MIN  = libtbag::graphic::channel_min();
    TBAG_CONSTEXPR static Channel const CHANNEL_HALF = libtbag::graphic::channel_half();

    Storage           _storage;
    sf::RenderWindow  _window;
    sf::Color         _clear;
    Activities        _activities;

    sol::table _lua_tbag;

    GameEngine(Storage & storage, sf::VideoMode const mode,
               std::string const & title, sf::Uint32 style,
               sf::ContextSettings const context, sf::Color const & clear)
            : _storage(storage), _window(mode, title, style, context),
              _clear(clear), _activities(_window)
    {
        if (initLuaBind(*_storage->lua.get())) {
            throw std::bad_alloc();
        }
        // _activities.push<MainMenuScene>();
    }

    virtual ~GameEngine()
    {
        // EMPTY.
    }

    bool run()
    {
        if (!onCreate()) {
            return false;
        }

        GameState state;
        sf::Event event;
        sf::Clock clock;
        sf::Time  delta;

        ImGui::SFML::Init(_window);

        while (_window.isOpen()) {
            delta = clock.restart();
            state.delta = delta.asMicroseconds();

            onCheck(state);

            while (_window.pollEvent(event)) {
                ImGui::SFML::ProcessEvent(event);
                onPreEvent(state);
                callEvnet((void*)&event, this);
                onPostEvent(state);
            }

            onUpdate(state);
            _activities.update(delta.asSeconds());
            onPostUpdate1(state);
            ImGui::SFML::Update(_window, delta);
            onPostUpdate2(state);

            _window.clear(_clear);
            onDraw(state);
            _activities.draw();
            onPostDraw1(state);
            ImGui::SFML::Render(_window);
            onPostDraw2(state);

            _window.display();
        }

        onDestroy();
        return true;
    }
};

int runGame(libtbag::res::Storage & storage, GameParams const & params)
{
    if (storage->lua_gui.empty()) {
        tDLogE("runGameMain() Entry point not defined.");
        return GAME_EXIT_CODE_EXIT_FAILURE;
    }
    if (!storage.runLuaScriptFile(storage->lua_gui)) {
        tDLogE("runGameMain() Lua script load failed: {}", storage->lua_gui);
        return GAME_EXIT_CODE_EXIT_FAILURE;
    }

    auto const MODE = sf::VideoMode(params.width, params.height, params.bpp);
    auto const STYLE = static_cast<sf::Uint32>(params.style);
    auto const CONTEXT = sf::ContextSettings(
            params.depth_bits,
            params.stencil_bits,
            params.antialiasing_level,
            params.major_version,
            params.minor_version,
            params.attribute_flags,
            params.srgb_capable);
    auto const CLEAR = sf::Color(params.clear_red, params.clear_green, params.clear_blue, params.clear_alpha);
    return GameEngine(storage, MODE, params.title, STYLE, CONTEXT, CLEAR).run();
}

int runGame(libtbag::res::Storage & storage)
{
    bool exit_game = false;
    int  exit_code = EXIT_FAILURE;

    while (!exit_game) {
        GameParams params;
        if (!readWindowParams(storage, params)) {
            tDLogW("runGame() Failed to load Window parameters.");
        }

        switch (runGame(storage, params)) {
        case GAME_EXIT_CODE_RESTART:
            break;
        case GAME_EXIT_CODE_EXIT_SUCCESS:
            exit_game = true;
            exit_code = EXIT_SUCCESS;
            break;
        case GAME_EXIT_CODE_EXIT_FAILURE:
            exit_game = true;
            exit_code = EXIT_FAILURE;
            break;
        default:
            TBAG_INACCESSIBLE_BLOCK_ASSERT();
            exit_game = true;
            exit_code = EXIT_FAILURE;
        }
    }

    return exit_code;
}

int runGame()
{
    libtbag::res::Storage storage;
    return runGame(storage);
}

} // namespace game

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

