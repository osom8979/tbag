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
#include <libtbag/Type.hpp>

#include <libtbag/game/WindowInterface.hpp>
#include <libtbag/game/WindowParams.hpp>

#include <libtbag/graphic/Color.hpp>
#include <libtbag/geometry/GeometryTypes.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/res/Storage.hpp>

#include <libtbag/3rd/imgui/imgui.h>
#include <libtbag/3rd/imgui/imgui-SFML.h>

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <Swoosh/ActivityController.h>
#include <Swoosh/Game.h>
#include <Swoosh/ActionList.h>
#include <Swoosh/Activity.h>
#include <Swoosh/Ease.h>
#include <Swoosh/EmbedGLSL.h>
#include <Swoosh/Segue.h>
#include <Swoosh/Shaders.h>
#include <Swoosh/Timer.h>

#include <Segues/BlackWashFade.h>
#include <Segues/BlendFadeIn.h>
#include <Segues/BlurFadeIn.h>
#include <Segues/Checkerboard.h>
#include <Segues/CircleClose.h>
#include <Segues/CircleOpen.h>
#include <Segues/CrossZoom.h>
#include <Segues/Cube3D.h>
#include <Segues/DiamondTileCircle.h>
#include <Segues/DiamondTileSwipe.h>
#include <Segues/HorizontalOpen.h>
#include <Segues/HorizontalSlice.h>
#include <Segues/Morph.h>
#include <Segues/PageTurn.h>
#include <Segues/PixelateBlackWashFade.h>
#include <Segues/PushIn.h>
#include <Segues/RadialCCW.h>
#include <Segues/RetroBlit.h>
#include <Segues/SlideIn.h>
#include <Segues/SwipeIn.h>
#include <Segues/VerticalOpen.h>
#include <Segues/VerticalSlice.h>
#include <Segues/WhiteWashFade.h>
#include <Segues/ZoomFadeIn.h>
#include <Segues/ZoomFadeInBounce.h>
#include <Segues/ZoomIn.h>
#include <Segues/ZoomOut.h>

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

enum class WindowExitCode
{
    WEC_RESTART = 0,
    WEC_EXIT_SUCCESS,
    WEC_EXIT_FAILURE,
};

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
struct Window : public WindowInterface, public libtbag::geometry::GeometryTypes
{
    STATIC_ASSERT_INTEGER_EQUAL(sf::Keyboard::Key::Unknown, Key::Unknown);
    STATIC_ASSERT_INTEGER_EQUAL(sf::Keyboard::Key::A,       Key::A);
    STATIC_ASSERT_INTEGER_EQUAL(sf::Keyboard::Key::Pause,   Key::Pause);

    STATIC_ASSERT_INTEGER_EQUAL(sf::Mouse::Button::Left,     Button::Left);
    STATIC_ASSERT_INTEGER_EQUAL(sf::Mouse::Button::Right,    Button::Right);
    STATIC_ASSERT_INTEGER_EQUAL(sf::Mouse::Button::Middle,   Button::Middle);
    STATIC_ASSERT_INTEGER_EQUAL(sf::Mouse::Button::XButton1, Button::XButton1);
    STATIC_ASSERT_INTEGER_EQUAL(sf::Mouse::Button::XButton2, Button::XButton2);

    STATIC_ASSERT_INTEGER_EQUAL(sf::Mouse::Wheel::VerticalWheel,   Wheel::VerticalWheel);
    STATIC_ASSERT_INTEGER_EQUAL(sf::Mouse::Wheel::HorizontalWheel, Wheel::HorizontalWheel);

    STATIC_ASSERT_INTEGER_EQUAL(sf::Joystick::Axis::X,    JoystickAxis::X);
    STATIC_ASSERT_INTEGER_EQUAL(sf::Joystick::Axis::Y,    JoystickAxis::Y);
    STATIC_ASSERT_INTEGER_EQUAL(sf::Joystick::Axis::Z,    JoystickAxis::Z);
    STATIC_ASSERT_INTEGER_EQUAL(sf::Joystick::Axis::R,    JoystickAxis::R);
    STATIC_ASSERT_INTEGER_EQUAL(sf::Joystick::Axis::U,    JoystickAxis::U);
    STATIC_ASSERT_INTEGER_EQUAL(sf::Joystick::Axis::V,    JoystickAxis::V);
    STATIC_ASSERT_INTEGER_EQUAL(sf::Joystick::Axis::PovX, JoystickAxis::PovX);
    STATIC_ASSERT_INTEGER_EQUAL(sf::Joystick::Axis::PovY, JoystickAxis::PovY);

    STATIC_ASSERT_INTEGER_EQUAL(sf::Sensor::Type::Accelerometer,    SensorType::Accelerometer);
    STATIC_ASSERT_INTEGER_EQUAL(sf::Sensor::Type::Gyroscope,        SensorType::Gyroscope);
    STATIC_ASSERT_INTEGER_EQUAL(sf::Sensor::Type::Magnetometer,     SensorType::Magnetometer);
    STATIC_ASSERT_INTEGER_EQUAL(sf::Sensor::Type::Gravity,          SensorType::Gravity);
    STATIC_ASSERT_INTEGER_EQUAL(sf::Sensor::Type::UserAcceleration, SensorType::UserAcceleration);
    STATIC_ASSERT_INTEGER_EQUAL(sf::Sensor::Type::Orientation,      SensorType::Orientation);

    using Channel = libtbag::graphic::Channel;
    using Rgb24   = libtbag::graphic::Rgb24;
    using Rgb32   = libtbag::graphic::Rgb32;

    TBAG_CONSTEXPR static Channel const CHANNEL_MAX  = libtbag::graphic::channel_max();
    TBAG_CONSTEXPR static Channel const CHANNEL_MIN  = libtbag::graphic::channel_min();
    TBAG_CONSTEXPR static Channel const CHANNEL_HALF = libtbag::graphic::channel_half();

    using Storage = libtbag::res::Storage;

    Storage          _storage;
    sf::RenderWindow _window;
    sf::Color        _clear;

    swoosh::ActivityController _activities;

    sol::state * _lua;
    sol::table   _lua_tbag;

    Window(Storage & storage, sf::VideoMode const mode,
           std::string const & title, sf::Uint32 style,
           sf::ContextSettings const context, sf::Color const & clear)
            : _storage(storage), _window(mode, title, style, context),
              _activities(_window), _clear(clear), _lua(_storage->lua.get())
    {
        // _activities.push<MainMenuScene>();

        assert(_lua != nullptr);
        _lua_tbag = (*_lua)[libtbag::script::SolState::lua_tbag_name()];
    }

    ~Window()
    {
        // EMPTY.
    }

    void runEvent(sf::Event & e)
    {
        // @formatter:off
        switch (e.type) {
        case sf::Event::Closed:
            onClosed();
            break;
        case sf::Event::Resized:
            onResized(e.size.width, e.size.height);
            break;
        case sf::Event::LostFocus:
            onLostFocus();
            break;
        case sf::Event::GainedFocus:
            onGainedFocus();
            break;
        case sf::Event::TextEntered:
            onTextEntered(e.text.unicode);
            break;
        case sf::Event::KeyPressed:
            onKeyPressed((Key)e.key.code, e.key.alt, e.key.control, e.key.shift, e.key.system);
            break;
        case sf::Event::KeyReleased:
            onKeyReleased((Key)e.key.code, e.key.alt, e.key.control, e.key.shift, e.key.system);
            break;
        case sf::Event::MouseWheelMoved:
            // DEPRECATED!
            break;
        case sf::Event::MouseWheelScrolled:
            onMouseWheelScrolled((Wheel)e.mouseWheelScroll.wheel, e.mouseWheelScroll.delta, e.mouseWheelScroll.x, e.mouseWheelScroll.y);
            break;
        case sf::Event::MouseButtonPressed:
            onMouseButtonPressed((Button)e.mouseButton.button, e.mouseButton.x, e.mouseButton.y);
            break;
        case sf::Event::MouseButtonReleased:
            onMouseButtonReleased((Button)e.mouseButton.button, e.mouseButton.x, e.mouseButton.y);
            break;
        case sf::Event::MouseMoved:
            onMouseMoved(e.mouseMove.x, e.mouseMove.y);
            break;
        case sf::Event::MouseEntered:
            onMouseEntered();
            break;
        case sf::Event::MouseLeft:
            onMouseLeft();
            break;
        case sf::Event::JoystickButtonPressed:
            onJoystickButtonPressed(e.joystickButton.joystickId, e.joystickButton.button);
            break;
        case sf::Event::JoystickButtonReleased:
            onJoystickButtonReleased(e.joystickButton.joystickId, e.joystickButton.button);
            break;
        case sf::Event::JoystickMoved:
            onJoystickMoved(e.joystickMove.joystickId, (JoystickAxis)e.joystickMove.axis, e.joystickMove.position);
            break;
        case sf::Event::JoystickConnected:
            onJoystickConnected(e.joystickConnect.joystickId);
            break;
        case sf::Event::JoystickDisconnected:
            onJoystickDisconnected(e.joystickConnect.joystickId);
            break;
        case sf::Event::TouchBegan:
            onTouchBegan(e.touch.finger, e.touch.x, e.touch.y);
            break;
        case sf::Event::TouchMoved:
            onTouchMoved(e.touch.finger, e.touch.x, e.touch.y);
            break;
        case sf::Event::TouchEnded:
            onTouchEnded(e.touch.finger, e.touch.x, e.touch.y);
            break;
        case sf::Event::SensorChanged:
            onSensorChanged((SensorType)e.sensor.type, e.sensor.x, e.sensor.y, e.sensor.z);
            break;
        default:
            TBAG_INACCESSIBLE_BLOCK_ASSERT();
            break;
        }
        // @formatter:on
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

    virtual void onCheck(WindowState & state) override
    {
    }

    virtual void onPreEvent(WindowState & state) override
    {
    }

    virtual void onPostEvent(WindowState & state) override
    {
    }

    virtual void onUpdate(WindowState & state) override
    {
    }

    virtual void onPreDraw(WindowState & state) override
    {
    }

    virtual void onPostDraw(WindowState & state) override
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

    virtual void onKeyPressed(Key code, bool alt, bool control, bool shift, bool system) override
    {
        _lua_tbag["onKeyPressed"](code, alt, control, shift, system);
    }

    virtual void onKeyReleased(Key code, bool alt, bool control, bool shift, bool system) override
    {
        _lua_tbag["onKeyReleased"](code, alt, control, shift, system);
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

    virtual void onMouseButtonPressed(Button button, int x, int y) override
    {
        _lua_tbag["onMouseButtonPressed"](button, x, y);
    }

    virtual void onMouseButtonReleased(Button button, int x, int y) override
    {
        _lua_tbag["onMouseButtonReleased"](button, x, y);
    }

    virtual void onMouseWheelScrolled(Wheel wheel, float delta, int x, int y) override
    {
        _lua_tbag["onMouseWheelScrolled"](wheel, delta, x, y);
    }

    virtual void onJoystickConnected(unsigned int joystick_id) override
    {
        _lua_tbag["onJoystickConnected"](joystick_id);
    }

    virtual void onJoystickDisconnected(unsigned int joystick_id) override
    {
        _lua_tbag["onJoystickDisconnected"](joystick_id);
    }

    virtual void onJoystickMoved(unsigned int joystick_id, JoystickAxis axis, float position) override
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

    virtual void onSensorChanged(SensorType type, float x, float y, float z) override
    {
        _lua_tbag["onSensorChanged"](type, x, y, z);
    }

    WindowExitCode run()
    {
        if (!onCreate()) {
            return WindowExitCode::WEC_EXIT_FAILURE;
        }

        WindowState state;
        sf::Clock   clock;
        sf::Event   event;
        sf::Time    delta;

        ImGui::SFML::Init(_window);

        while (_window.isOpen()) {
            delta = clock.restart();
            state.delta = delta.asMicroseconds();

            onCheck(state);

            while (_window.pollEvent(event)) {
                ImGui::SFML::ProcessEvent(event);
                onPreEvent(state);
                runEvent(event);
                onPostEvent(state);
            }

            onUpdate(state);
            _activities.update(delta.asSeconds());
            ImGui::SFML::Update(_window, delta);

            _window.clear(_clear);
            onPreDraw(state);
            _activities.draw();
            ImGui::SFML::Render(_window);
            onPostDraw(state);

            _window.display();
        }

        onDestroy();

        return WindowExitCode::WEC_EXIT_SUCCESS;
    }
};

static WindowExitCode runGameMain(libtbag::res::Storage & storage, WindowParams const & params)
{
    if (storage->lua_gui.empty()) {
        tDLogE("runGameMain() Entry point not defined.");
        return WindowExitCode::WEC_EXIT_FAILURE;
    }

    if (!storage.runLuaScriptFile(storage->lua_gui)) {
        tDLogE("runGameMain() Lua script load failed: {}", storage->lua_gui);
        return WindowExitCode::WEC_EXIT_FAILURE;
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
    return Window(storage, MODE, LIBTBAG_MAIN_TITLE, STYLE, CONTEXT, CLEAR).run();
}

int runGame(libtbag::res::Storage & storage)
{
    bool exit_game = false;
    int  exit_code = EXIT_FAILURE;

    while (!exit_game) {
        WindowParams params;
        if (!readWindowParams(storage, params)) {
            tDLogW("runGame() Failed to load Window parameters.");
        }

        switch (runGameMain(storage, params)) {
        case WindowExitCode::WEC_RESTART:
            break;
        case WindowExitCode::WEC_EXIT_SUCCESS:
            exit_game = true;
            exit_code = EXIT_SUCCESS;
            break;
        case WindowExitCode::WEC_EXIT_FAILURE:
            exit_game = true;
            exit_code = EXIT_FAILURE;
            break;
        }
    }

    return exit_code;
}

} // namespace game

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

