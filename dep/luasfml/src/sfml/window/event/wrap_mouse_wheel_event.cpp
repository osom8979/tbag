#include "wrap_mouse_wheel_event.h"

#include <lua.hpp>

#include "../../../bindingHelper/marco.hpp"
#include "../../../bindingHelper/lua_template.hpp"


#define TYPE_TO_BIND sf::Event::MouseWheelEvent
#define NAME_TO_BIND sfMouseWheelEvent

IMPLEMENT_LUA_CLASS(sfMouseWheelEvent,sf::Event::MouseWheelEvent)

#define FUNCTION_TO_BIND \
    X0(x      , GETTER   ,HAS_RETURN , RET_TYPE(int) , x       )\
    X0(y      , GETTER   ,HAS_RETURN , RET_TYPE(int) , y       )\
    X0(delta  , GETTER   ,HAS_RETURN , RET_TYPE(int) , delta   )\

#define __GO_FOR_IMPLEMENTATION__
#include "../../../bindingHelper/macro2.hpp"

FUNCTION_TO_BIND

#undef __GO_FOR_IMPLEMENTATION__
#include "../../../bindingHelper/macro2.hpp"

luaL_Reg sfMouseWheelEvent_regs[] =
{
    LUA_DEFAULT_CLASS_FUNC(sfMouseWheelEvent)
    FUNCTION_TO_BIND
    { NULL, NULL }
};

LUA_REGISTER_CLASS(sfMouseWheelEvent)
