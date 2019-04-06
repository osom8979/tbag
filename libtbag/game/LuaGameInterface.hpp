/**
 * @file   LuaGameInterface.hpp
 * @brief  LuaGameInterface class prototype.
 * @author zer0
 * @date   2019-04-06
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GAME_LUAGAMEINTERFACE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GAME_LUAGAMEINTERFACE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/game/GameCommon.hpp>
#include <libtbag/script/Sol2Bypass.hpp>

#include <functional>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace game {

/**
 * LuaGameInterface class prototype.
 *
 * @author zer0
 * @date   2019-04-06
 */
class TBAG_API LuaGameInterface : public libtbag::game::GameInterface
{
private:
    // clang-format off
    std::function<bool(void)>                        _on_create;
    std::function<void(void)>                        _on_destroy;
    std::function<void(std::int64_t)>                _on_check;
    std::function<void(std::int64_t)>                _on_pre_event;
    std::function<void(std::int64_t)>                _on_post_event;
    std::function<void(std::int64_t)>                _on_update;
    std::function<void(std::int64_t)>                _on_post_update1;
    std::function<void(std::int64_t)>                _on_post_update2;
    std::function<void(std::int64_t)>                _on_draw;
    std::function<void(std::int64_t)>                _on_post_draw1;
    std::function<void(std::int64_t)>                _on_post_draw2;
    std::function<bool(void)>                        _on_close;
    std::function<void(unsigned, unsigned)>          _on_resized;
    std::function<void(void)>                        _on_lost_focus;
    std::function<void()>                            _on_gained_focus;
    std::function<void(int, bool, bool, bool, bool)> _on_key_pressed;
    std::function<void(int, bool, bool, bool, bool)> _on_key_released;
    std::function<void(unsigned)>                    _on_textEntered;
    std::function<void(int, int)>                    _on_mouse_moved;
    std::function<void(void)>                        _on_mouse_entered;
    std::function<void(void)>                        _on_mouse_left;
    std::function<void(int, int, int)>               _on_mouse_button_pressed;
    std::function<void(int, int, int)>               _on_mouse_button_released;
    std::function<void(int, float, int, int)>        _on_mouse_wheel_scrolled;
    std::function<void(unsigned)>                    _on_joystick_connected;
    std::function<void(unsigned)>                    _on_joystick_disconnected;
    std::function<void(unsigned, int, float)>        _on_joystick_moved;
    std::function<void(unsigned, unsigned)>          _on_joystick_button_pressed;
    std::function<void(unsigned, unsigned)>          _on_joystick_button_released;
    std::function<void(unsigned, int, int)>          _on_touch_began;
    std::function<void(unsigned, int, int)>          _on_touch_moved;
    std::function<void(unsigned, int, int)>          _on_touch_ended;
    std::function<void(int, float, float, float)>    _on_sensor_changed;
    // clang-format on

public:
    LuaGameInterface();
    LuaGameInterface(sol::state & lua);
    virtual ~LuaGameInterface();

protected:
    bool initLuaBind(sol::state & lua);

private:
    template <typename CallbackT>
    static void _bind_lua_cb(sol::table & lua_tbag, char const * name, CallbackT & cb)
    {
        sol::protected_function lua_function = lua_tbag[name];
        if (lua_function.valid()) {
            cb = lua_function;
        }
    }

protected:
    virtual void closeWindow();

protected:
    virtual bool onCreate() override;
    virtual void onDestroy() override;
    virtual void onCheck(GameState & state) override;
    virtual void onPreEvent(GameState & state) override;
    virtual void onPostEvent(GameState & state) override;
    virtual void onUpdate(GameState & state) override;
    virtual void onPostUpdate1(GameState & state) override;
    virtual void onPostUpdate2(GameState & state) override;
    virtual void onDraw(GameState & state) override;
    virtual void onPostDraw1(GameState & state) override;
    virtual void onPostDraw2(GameState & state) override;
    virtual void onClosed() override;
    virtual void onResized(unsigned int width, unsigned int height) override;
    virtual void onLostFocus() override;
    virtual void onGainedFocus() override;
    virtual void onKeyPressed(GameKey code, bool alt, bool control, bool shift, bool system) override;
    virtual void onKeyReleased(GameKey code, bool alt, bool control, bool shift, bool system) override;
    virtual void onTextEntered(unsigned int unicode) override;
    virtual void onMouseMoved(int x, int y) override;
    virtual void onMouseEntered() override;
    virtual void onMouseLeft() override;
    virtual void onMouseButtonPressed(GameButton button, int x, int y) override;
    virtual void onMouseButtonReleased(GameButton button, int x, int y) override;
    virtual void onMouseWheelScrolled(GameWheel wheel, float delta, int x, int y) override;
    virtual void onJoystickConnected(unsigned int joystick_id) override;
    virtual void onJoystickDisconnected(unsigned int joystick_id) override;
    virtual void onJoystickMoved(unsigned int joystick_id, GameJoystickAxis axis, float position) override;
    virtual void onJoystickButtonPressed(unsigned int joystick_id, unsigned int button) override;
    virtual void onJoystickButtonReleased(unsigned int joystick_id, unsigned int button) override;
    virtual void onTouchBegan(unsigned int finger, int x, int y) override;
    virtual void onTouchMoved(unsigned int finger, int x, int y) override;
    virtual void onTouchEnded(unsigned int finger, int x, int y) override;
    virtual void onSensorChanged(GameSensorType type, float x, float y, float z) override;
};

} // namespace game

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GAME_LUAGAMEINTERFACE_HPP__

