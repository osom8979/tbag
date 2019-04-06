/**
 * @file   LuaGameInterface.cpp
 * @brief  LuaGameInterface class implementation.
 * @author zer0
 * @date   2019-04-06
 */

#include <libtbag/game/LuaGameInterface.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace game {

LuaGameInterface::LuaGameInterface()
{
    // EMPTY.
}

LuaGameInterface::LuaGameInterface(sol::state & lua) : LuaGameInterface()
{
    if (!initLuaBind(lua)) {
        throw std::bad_alloc();
    }
}

LuaGameInterface::~LuaGameInterface()
{
    // EMPTY.
}

bool LuaGameInterface::initLuaBind(sol::state & lua)
{
    // clang-format off
    sol::table lua_tbag = lua[libtbag::script::SolState::lua_tbag_name()];
    _bind_lua_cb(lua_tbag, "on_create",                    _on_create);
    _bind_lua_cb(lua_tbag, "on_destroy",                   _on_destroy);
    _bind_lua_cb(lua_tbag, "on_check",                     _on_check);
    _bind_lua_cb(lua_tbag, "on_pre_event",                 _on_pre_event);
    _bind_lua_cb(lua_tbag, "on_post_event",                _on_post_event);
    _bind_lua_cb(lua_tbag, "on_update",                    _on_update);
    _bind_lua_cb(lua_tbag, "on_post_update1",              _on_post_update1);
    _bind_lua_cb(lua_tbag, "on_post_update2",              _on_post_update2);
    _bind_lua_cb(lua_tbag, "on_draw",                      _on_draw);
    _bind_lua_cb(lua_tbag, "on_post_draw1",                _on_post_draw1);
    _bind_lua_cb(lua_tbag, "on_post_draw2",                _on_post_draw2);
    _bind_lua_cb(lua_tbag, "on_closed",                    _on_close);
    _bind_lua_cb(lua_tbag, "on_resized",                  _on_resized);
    _bind_lua_cb(lua_tbag, "on_lost_focus",               _on_lost_focus);
    _bind_lua_cb(lua_tbag, "on_gained_focus",             _on_gained_focus);
    _bind_lua_cb(lua_tbag, "on_key_pressed",              _on_key_pressed);
    _bind_lua_cb(lua_tbag, "on_key_released",             _on_key_released);
    _bind_lua_cb(lua_tbag, "on_textEntered",              _on_textEntered);
    _bind_lua_cb(lua_tbag, "on_mouse_moved",              _on_mouse_moved);
    _bind_lua_cb(lua_tbag, "on_mouse_entered",            _on_mouse_entered);
    _bind_lua_cb(lua_tbag, "on_mouse_left",               _on_mouse_left);
    _bind_lua_cb(lua_tbag, "on_mouse_button_pressed",     _on_mouse_button_pressed);
    _bind_lua_cb(lua_tbag, "on_mouse_button_released",    _on_mouse_button_released);
    _bind_lua_cb(lua_tbag, "on_mouse_wheel_scrolled",     _on_mouse_wheel_scrolled);
    _bind_lua_cb(lua_tbag, "on_joystick_connected",       _on_joystick_connected);
    _bind_lua_cb(lua_tbag, "on_joystick_disconnected",    _on_joystick_disconnected);
    _bind_lua_cb(lua_tbag, "on_joystick_moved",           _on_joystick_moved);
    _bind_lua_cb(lua_tbag, "on_joystick_button_pressed",  _on_joystick_button_pressed);
    _bind_lua_cb(lua_tbag, "on_joystick_button_released", _on_joystick_button_released);
    _bind_lua_cb(lua_tbag, "on_touch_began",              _on_touch_began);
    _bind_lua_cb(lua_tbag, "on_touch_moved",              _on_touch_moved);
    _bind_lua_cb(lua_tbag, "on_touch_ended",              _on_touch_ended);
    _bind_lua_cb(lua_tbag, "on_sensor_changed",           _on_sensor_changed);
    // clang-format on
    return true;
}

bool LuaGameInterface::onCreate()
{
    if (_on_create) {
        return _on_create();
    }
    return true;
}

void LuaGameInterface::onDestroy()
{
    if (_on_destroy) {
        _on_destroy();
    }
}

void LuaGameInterface::onCheck(GameState & state)
{
    if (_on_check) {
        _on_check(state.delta);
    }
}

void LuaGameInterface::onPreEvent(GameState & state)
{
    if (_on_pre_event) {
        _on_pre_event(state.delta);
    }
}

void LuaGameInterface::onPostEvent(GameState & state)
{
    if (_on_post_event) {
        _on_post_event(state.delta);
    }
}

void LuaGameInterface::onUpdate(GameState & state)
{
    if (_on_update) {
        _on_update(state.delta);
    }
}

void LuaGameInterface::onPostUpdate1(GameState & state)
{
    if (_on_post_update1) {
        _on_post_update1(state.delta);
    }
}

void LuaGameInterface::onPostUpdate2(GameState & state)
{
    if (_on_post_update2) {
        _on_post_update2(state.delta);
    }
}

void LuaGameInterface::onDraw(GameState & state)
{
    if (_on_draw) {
        _on_draw(state.delta);
    }
}

void LuaGameInterface::onPostDraw1(GameState & state)
{
    if (_on_post_draw1) {
        _on_post_draw1(state.delta);
    }
}

void LuaGameInterface::onPostDraw2(GameState & state)
{
    if (_on_post_draw2) {
        _on_post_draw2(state.delta);
    }
}

void LuaGameInterface::onClosed()
{
    bool exit_window = true;
    if (_on_close) {
        exit_window = _on_close();
    }
    if (exit_window) {
        closeGame();
    }
}

void LuaGameInterface::onResized(unsigned int width, unsigned int height)
{
    if (_on_resized) {
        _on_resized(width, height);
    }
}

void LuaGameInterface::onLostFocus()
{
    if (_on_lost_focus) {
        _on_lost_focus();
    }
}

void LuaGameInterface::onGainedFocus()
{
    if (_on_gained_focus) {
        _on_gained_focus();
    }
}

void LuaGameInterface::onKeyPressed(GameKey code, bool alt, bool control, bool shift, bool system)
{
    if (_on_key_pressed) {
        _on_key_pressed((int)code, alt, control, shift, system);
    }
}

void LuaGameInterface::onKeyReleased(GameKey code, bool alt, bool control, bool shift, bool system)
{
    if (_on_key_released) {
        _on_key_released((int)code, alt, control, shift, system);
    }
}

void LuaGameInterface::onTextEntered(unsigned int unicode)
{
    if (_on_textEntered) {
        _on_textEntered(unicode);
    }
}

void LuaGameInterface::onMouseMoved(int x, int y)
{
    if (_on_mouse_moved) {
        _on_mouse_moved(x, y);
    }
}

void LuaGameInterface::onMouseEntered()
{
    if (_on_mouse_entered) {
        _on_mouse_entered();
    }
}

void LuaGameInterface::onMouseLeft()
{
    if (_on_mouse_left) {
        _on_mouse_left();
    }
}

void LuaGameInterface::onMouseButtonPressed(GameButton button, int x, int y)
{
    if (_on_mouse_button_pressed) {
        _on_mouse_button_pressed((int)button, x, y);
    }
}

void LuaGameInterface::onMouseButtonReleased(GameButton button, int x, int y)
{
    if (_on_mouse_button_released) {
        _on_mouse_button_released((int)button, x, y);
    }
}

void LuaGameInterface::onMouseWheelScrolled(GameWheel wheel, float delta, int x, int y)
{
    if (_on_mouse_wheel_scrolled) {
        _on_mouse_wheel_scrolled((int)wheel, delta, x, y);
    }
}

void LuaGameInterface::onJoystickConnected(unsigned int joystick_id)
{
    if (_on_joystick_connected) {
        _on_joystick_connected(joystick_id);
    }
}

void LuaGameInterface::onJoystickDisconnected(unsigned int joystick_id)
{
    if (_on_joystick_disconnected) {
        _on_joystick_disconnected(joystick_id);
    }
}

void LuaGameInterface::onJoystickMoved(unsigned int joystick_id, GameJoystickAxis axis, float position)
{
    if (_on_joystick_moved) {
        _on_joystick_moved(joystick_id, (int)axis, position);
    }
}

void LuaGameInterface::onJoystickButtonPressed(unsigned int joystick_id, unsigned int button)
{
    if (_on_joystick_button_pressed) {
        _on_joystick_button_pressed(joystick_id, button);
    }
}

void LuaGameInterface::onJoystickButtonReleased(unsigned int joystick_id, unsigned int button)
{
    if (_on_joystick_button_released) {
        _on_joystick_button_released(joystick_id, button);
    }
}

void LuaGameInterface::onTouchBegan(unsigned int finger, int x, int y)
{
    if (_on_touch_began) {
        _on_touch_began(finger, x, y);
    }
}

void LuaGameInterface::onTouchMoved(unsigned int finger, int x, int y)
{
    if (_on_touch_moved) {
        _on_touch_moved(finger, x, y);
    }
}

void LuaGameInterface::onTouchEnded(unsigned int finger, int x, int y)
{
    if (_on_touch_ended) {
        _on_touch_ended(finger, x, y);
    }
}

void LuaGameInterface::onSensorChanged(GameSensorType type, float x, float y, float z)
{
    if (_on_sensor_changed) {
        _on_sensor_changed((int)type, x, y, z);
    }
}

} // namespace game

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

