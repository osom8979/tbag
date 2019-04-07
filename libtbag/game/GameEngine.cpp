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
#include <libtbag/log/Log.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/res/Storage.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/Type.hpp>

#include <libtbag/game/gui/GuiCircle.hpp>
#include <libtbag/game/gui/GuiRect.hpp>
#include <libtbag/game/gui/GuiText.hpp>

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

/**
 * GameEngine backend implementation.
 *
 * @author zer0
 * @date   2019-03-23
 * @date   2019-04-06 (Rename: Window -> GameEngine)
 */
struct GameEngine::Impl : private Noncopyable
{
    GameEngine *      parent;
    sf::RenderWindow  window;
    sf::Color         clear_color;
    Activities        activities;

    Impl(GameEngine * p, Storage & storage, sf::VideoMode const mode, std::string const & title,
         sf::Uint32 style, sf::ContextSettings const context, sf::Color const & clear)
            : parent(p), window(mode, title, style, context), clear_color(clear), activities(window)
    {
        assert(parent != nullptr);
    }

    ~Impl()
    {
        // EMPTY.
    }

    void closeGame()
    {
        window.close();
    }

    bool run()
    {
        assert(parent != nullptr);
        if (!parent->onCreate()) {
            return false;
        }

        GameState state;
        sf::Event event;
        sf::Clock clock;
        sf::Time  delta;

        ImGui::SFML::Init(window);

        while (window.isOpen()) {
            delta = clock.restart();
            state.delta = delta.asMicroseconds();

            parent->onCheck(state);

            while (window.pollEvent(event)) {
                ImGui::SFML::ProcessEvent(event);
                parent->onPreEvent(state);
                callEvnet((void*)&event, parent);
                parent->onPostEvent(state);
            }

            parent->onUpdate(state);
            activities.update(delta.asSeconds());
            parent->onPostUpdate1(state);
            ImGui::SFML::Update(window, delta);
            parent->onPostUpdate2(state);

            window.clear(clear_color);
            parent->onDraw(state);
            activities.draw();
            parent->onPostDraw1(state);
            ImGui::SFML::Render(window);
            parent->onPostDraw2(state);

            window.display();
        }

        parent->onDestroy();
        return true;
    }
};

// -------------------------
// GameEngine implementation
// -------------------------

GameEngine::GameEngine()
        : _storage(), _params(), _impl(nullptr)
{
    // EMPTY.
}

GameEngine::GameEngine(Storage const & storage)
        : _storage(storage), _params(), _impl(nullptr)
{
    // EMPTY.
}

GameEngine::GameEngine(Storage const & storage, GameParams const & params)
        : _storage(storage), _params(params), _impl(nullptr)
{
    // EMPTY.
}

GameEngine::~GameEngine()
{
    // EMPTY.
}

void GameEngine::closeGame()
{
    if (_impl) {
        _impl->closeGame();
    }
}

int GameEngine::run()
{
    //if (_storage->lua_gui.empty()) {
    //    tDLogE("runGame() Entry point not defined.");
    //    return GAME_EXIT_CODE_EXIT_FAILURE;
    //}
    //if (!_storage.runLuaScriptFile(_storage->lua_gui)) {
    //    tDLogE("runGame() Lua script load failed: {}", storage->lua_gui);
    //    return GAME_EXIT_CODE_EXIT_FAILURE;
    //}

    auto const PARAMS = _params;
    auto const MODE = sf::VideoMode(PARAMS.width, PARAMS.height, PARAMS.bpp);
    auto const STYLE = static_cast<sf::Uint32>(PARAMS.style);
    auto const CONTEXT = sf::ContextSettings(
            PARAMS.depth_bits,
            PARAMS.stencil_bits,
            PARAMS.antialiasing_level,
            PARAMS.major_version,
            PARAMS.minor_version,
            PARAMS.attribute_flags,
            PARAMS.srgb_capable);
    auto const CLEAR = sf::Color(PARAMS.clear_red, PARAMS.clear_green, PARAMS.clear_blue, PARAMS.clear_alpha);
    _impl = std::make_unique<Impl>(this, _storage, MODE, PARAMS.title, STYLE, CONTEXT, CLEAR);
    assert(static_cast<bool>(_impl));
    return _impl->run();
}

void GameEngine::draw(GuiCircle const & gui)
{
    assert(static_cast<bool>(_impl));
    _impl->window.draw(*(sf::CircleShape const *)gui.get());
}

void GameEngine::draw(GuiRect const & gui)
{
    assert(static_cast<bool>(_impl));
    _impl->window.draw(*(sf::RectangleShape const *)gui.get());
}

void GameEngine::draw(GuiText const & gui)
{
    assert(static_cast<bool>(_impl));
    _impl->window.draw(*(sf::Text const *)gui.get());
}

/**
 * GameScene backend implementation.
 *
 * @author zer0
 * @date   2019-04-06
 */
struct GameScene::Impl : private Noncopyable, public Activity
{
    GameEngine * engine;
    GameScene * scene;

    Impl(GameEngine * e, GameScene * s) : Activity(&(e->get()->activities)), engine(e), scene(s)
    {
        assert(engine != nullptr);
        assert(scene != nullptr);
    }

    virtual ~Impl()
    {
        // EMPTY.
    }

    virtual void onStart() override
    {
        assert(scene != nullptr);
        scene->onStart();
    }

    virtual void onUpdate(double elapsed) override
    {
        assert(scene != nullptr);
        scene->onUpdate(elapsed);
    }

    virtual void onLeave() override
    {
        assert(scene != nullptr);
        scene->onLeave();
    }

    virtual void onExit() override
    {
        assert(scene != nullptr);
        scene->onExit();
    }

    virtual void onEnter() override
    {
        assert(scene != nullptr);
        scene->onEnter();
    }

    virtual void onResume() override
    {
        assert(scene != nullptr);
        scene->onResume();
    }

    virtual void onDraw(sf::RenderTexture & surface) override
    {
        assert(scene != nullptr);
        scene->onDraw();
    }

    virtual void onEnd() override
    {
        assert(scene != nullptr);
        scene->onEnd();
    }
};

// ------------------------
// GameScene implementation
// ------------------------

GameScene::GameScene(GameEngine * engine)
        : _impl(std::make_unique<Impl>(engine, this))
{
    // EMPTY.
}

GameScene::~GameScene()
{
    // EMPTY.
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

        switch (GameEngine(storage, params).run()) {
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

