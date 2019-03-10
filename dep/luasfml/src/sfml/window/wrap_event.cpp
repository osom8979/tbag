#include "wrap_event.h"
#include <lua.hpp>
#include "../../bindingHelper/enum.hpp"


#include "../window/event/wrap_key_event.h"
#include "../window/event/wrap_mouse_button_event.h"
#include "../window/event/wrap_mouse_move_event.h"
#include "../window/event/wrap_mouse_wheel_event.h"
#include "../window/event/wrap_size_event.h"

void register_EventEnums(lua_State * l)
{
    add_enum_to_lua(l,"sfEventType",
		    "Closed"                 , sf::Event::Closed,
		    "Resized"                , sf::Event::Resized,
		    "LostFocus"              , sf::Event::LostFocus,
		    "GainedFocus"            , sf::Event::GainedFocus,
		    "TextEntered"            , sf::Event::TextEntered,
		    "KeyPressed"             , sf::Event::KeyPressed,
		    "KeyReleased"            , sf::Event::KeyReleased,
		    "MouseWheelMoved"        , sf::Event::MouseWheelMoved,
		    "MouseButtonPressed"     , sf::Event::MouseButtonPressed,
		    "MouseButtonReleased"    , sf::Event::MouseButtonReleased,
		    "MouseMoved"             , sf::Event::MouseMoved,
		    "MouseLeft"              , sf::Event::MouseLeft,
		    "JoystickButtonPressed"  , sf::Event::JoystickButtonPressed,
		    "JoystickButtonReleased" , sf::Event::JoystickButtonReleased,
		    "JoystickMoved"          , sf::Event::JoystickMoved,
		    "JoystickConnected"      , sf::Event::JoystickConnected,
		    "JoystickDisconnected"   , sf::Event::JoystickDisconnected,
		    "Count"                  , sf::Event::Count,
		    nullptr);

}

#include <lua.hpp>

#include "../../bindingHelper/marco.hpp"
#include "../../bindingHelper/lua_template.hpp"


#define TYPE_TO_BIND sf::Event
#define NAME_TO_BIND sfEvent

IMPLEMENT_LUA_CLASS(sfEvent,sf::Event)

#define FUNCTION_TO_BIND \
    X0(type               , GETTER   ,HAS_RETURN , RET_TYPE(                         int) , type                )\
    X0(size               , GETTER   ,HAS_RETURN , RET_TYPE( sf::Event::SizeEvent       ) , size                )\
    X0(key                , GETTER   ,HAS_RETURN , RET_TYPE( sf::Event::KeyEvent        ) , key                 )\
    X0(mouseMove          , GETTER   ,HAS_RETURN , RET_TYPE( sf::Event::MouseMoveEvent  ) , mouseMove           )\
    X0(mouseButton        , GETTER   ,HAS_RETURN , RET_TYPE( sf::Event::MouseButtonEvent) , mouseButton         )\
    X0(mouseWheel         , GETTER   ,HAS_RETURN , RET_TYPE( sf::Event::MouseWheelEvent ) , mouseWheel          )\

#define __GO_FOR_IMPLEMENTATION__
#include "../../bindingHelper/macro2.hpp"

FUNCTION_TO_BIND

#undef __GO_FOR_IMPLEMENTATION__
#include "../../bindingHelper/macro2.hpp"

luaL_Reg sfEvent_regs[] =
{
    LUA_DEFAULT_CLASS_FUNC(sfEvent)
    FUNCTION_TO_BIND
    { NULL, NULL }
};

LUA_REGISTER_CLASS(sfEvent)



