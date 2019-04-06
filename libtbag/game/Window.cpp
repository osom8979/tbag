/**
 * @file   Window.cpp
 * @brief  Window class implementation.
 * @author zer0
 * @date   2019-01-31
 * @date   2019-02-13 (Rename: Window -> SfWindow)
 * @date   2019-02-16 (Rename: SfWindow -> RenderWindow)
 * @date   2019-03-23 (Rename: RenderWindow -> Window)
 */

#include <libtbag/game/Window.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/res/Storage.hpp>
#include <libtbag/geometry/GeometryTypes.hpp>
#include <libtbag/graphic/Color.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/Type.hpp>

#include <libtbag/3rd/imgui/imgui.h>
#include <libtbag/3rd/imgui/imgui-SFML.h>

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

struct Scene : public swoosh::Activity
{
    Scene(swoosh::ActivityController * controller) : swoosh::Activity(controller)
    {
        // EMPTY.
    }

    virtual ~Scene()
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
 * Window backend implementation.
 *
 * @author zer0
 * @date   2019-03-23
 */
struct Window : public GameInterface, public libtbag::geometry::GeometryTypes
{
    using Channel = libtbag::graphic::Channel;
    using Rgb24   = libtbag::graphic::Rgb24;
    using Rgb32   = libtbag::graphic::Rgb32;

    using Storage = libtbag::res::Storage;
    using Activities = swoosh::ActivityController;

    TBAG_CONSTEXPR static Channel const CHANNEL_MAX  = libtbag::graphic::channel_max();
    TBAG_CONSTEXPR static Channel const CHANNEL_MIN  = libtbag::graphic::channel_min();
    TBAG_CONSTEXPR static Channel const CHANNEL_HALF = libtbag::graphic::channel_half();

    Storage          _storage;
    sf::RenderWindow _window;
    sf::Color        _clear;
    Activities       _activities;

    sol::state * _lua;
    sol::table   _lua_tbag;

    Window(Storage & storage, sf::VideoMode const mode,
           std::string const & title, sf::Uint32 style,
           sf::ContextSettings const context, sf::Color const & clear)
            : _storage(storage), _window(mode, title, style, context),
              _clear(clear), _activities(_window), _lua(_storage->lua.get())
    {
        assert(_lua != nullptr);
        _lua_tbag = (*_lua)[libtbag::script::SolState::lua_tbag_name()];

        // _activities.push<MainMenuScene>();
    }

    ~Window()
    {
        // EMPTY.
    }

    virtual bool onCreate() override
    {
        sol::protected_function lua_function = _lua_tbag["onCreate"];
        if (lua_function.valid()) {
            std::function<bool(void)> std_function = lua_function;
            return std_function();
        }
        return true;
    }

    virtual void onDestroy() override
    {
        sol::protected_function lua_function = _lua_tbag["onDestroy"];
        if (lua_function.valid()) {
            std::function<void(void)> std_function = lua_function;
            std_function();
        }
    }

    virtual void onCheck(GameState & state) override
    {
    }

    virtual void onPreEvent(GameState & state) override
    {
    }

    virtual void onPostEvent(GameState & state) override
    {
    }

    virtual void onUpdate(GameState & state) override
    {
    }

    virtual void onDraw(GameState & state) override
    {
    }

    virtual void onPostDraw1(GameState & state) override
    {
    }

    virtual void onPostDraw2(GameState & state) override
    {
    }

    virtual void onClosed() override
    {
        bool exit_window = true;
        sol::protected_function lua_function = _lua_tbag["onClosed"];
        if (lua_function.valid()) {
            std::function<bool(void)> std_function = lua_function;
            exit_window = std_function();
        }
        if (exit_window) {
            _window.close();
        }
    }

    virtual void onResized(unsigned int width, unsigned int height) override
    {
        _lua_tbag["onResized"](width, height);
    }

    virtual void onLostFocus() override
    {
        _lua_tbag["onLostFocus"]();
    }

    virtual void onGainedFocus() override
    {
        _lua_tbag["onGainedFocus"]();
    }

    virtual void onKeyPressed(GameKey code, bool alt, bool control, bool shift, bool system) override
    {
        _lua_tbag["onKeyPressed"]((int)code, alt, control, shift, system);
    }

    virtual void onKeyReleased(GameKey code, bool alt, bool control, bool shift, bool system) override
    {
        _lua_tbag["onKeyReleased"]((int)code, alt, control, shift, system);
    }

    virtual void onTextEntered(unsigned int unicode) override
    {
        _lua_tbag["onTextEntered"](unicode);
    }

    virtual void onMouseMoved(int x, int y) override
    {
        _lua_tbag["onMouseMoved"](x, y);
    }

    virtual void onMouseEntered() override
    {
        _lua_tbag["onMouseEntered"]();
    }

    virtual void onMouseLeft() override
    {
        _lua_tbag["onMouseLeft"]();
    }

    virtual void onMouseButtonPressed(GameButton button, int x, int y) override
    {
        _lua_tbag["onMouseButtonPressed"]((int)button, x, y);
    }

    virtual void onMouseButtonReleased(GameButton button, int x, int y) override
    {
        _lua_tbag["onMouseButtonReleased"]((int)button, x, y);
    }

    virtual void onMouseWheelScrolled(GameWheel wheel, float delta, int x, int y) override
    {
        _lua_tbag["onMouseWheelScrolled"]((int)wheel, delta, x, y);
    }

    virtual void onJoystickConnected(unsigned int joystick_id) override
    {
        _lua_tbag["onJoystickConnected"](joystick_id);
    }

    virtual void onJoystickDisconnected(unsigned int joystick_id) override
    {
        _lua_tbag["onJoystickDisconnected"](joystick_id);
    }

    virtual void onJoystickMoved(unsigned int joystick_id, GameJoystickAxis axis, float position) override
    {
        _lua_tbag["onJoystickMoved"](joystick_id, axis, position);
    }

    virtual void onJoystickButtonPressed(unsigned int joystick_id, unsigned int button) override
    {
        _lua_tbag["onJoystickButtonPressed"](joystick_id, button);
    }

    virtual void onJoystickButtonReleased(unsigned int joystick_id, unsigned int button) override
    {
        _lua_tbag["onJoystickButtonReleased"](joystick_id, button);
    }

    virtual void onTouchBegan(unsigned int finger, int x, int y) override
    {
        _lua_tbag["onTouchBegan"](finger, x, y);
    }

    virtual void onTouchMoved(unsigned int finger, int x, int y) override
    {
        _lua_tbag["onTouchMoved"](finger, x, y);
    }

    virtual void onTouchEnded(unsigned int finger, int x, int y) override
    {
        _lua_tbag["onTouchEnded"](finger, x, y);
    }

    virtual void onSensorChanged(GameSensorType type, float x, float y, float z) override
    {
        _lua_tbag["onSensorChanged"]((int)type, x, y, z);
    }

    bool run()
    {
        if (!onCreate()) {
            return false;
        }

        GameState state;
        sf::Clock clock;
        sf::Event event;
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

int runGameMain(libtbag::res::Storage & storage, GameParams const & params)
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
    return Window(storage, MODE, params.title, STYLE, CONTEXT, CLEAR).run();
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

        switch (runGameMain(storage, params)) {
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

