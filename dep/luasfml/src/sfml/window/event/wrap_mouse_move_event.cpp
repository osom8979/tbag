#include "wrap_mouse_move_event.h"
#include <lua.hpp>
#include <SFML/Window/Event.hpp>

#include "../../../bindingHelper/marco.hpp"
#include "../../../bindingHelper/lua_template.hpp"


#define TYPE_TO_BIND sf::Event::MouseMoveEvent
#define NAME_TO_BIND sfMouseMoveEvent

IMPLEMENT_LUA_CLASS(sfMouseMoveEvent,sf::Event::MouseMoveEvent)

#define FUNCTION_TO_BIND \
    X0(x      , GETTER   ,HAS_RETURN , RET_TYPE(int) , x       )\
    X0(y      , GETTER   ,HAS_RETURN , RET_TYPE(int) , y       )\


#define __GO_FOR_IMPLEMENTATION__
#include "../../../bindingHelper/macro2.hpp"

FUNCTION_TO_BIND

#undef __GO_FOR_IMPLEMENTATION__
#include "../../../bindingHelper/macro2.hpp"

luaL_Reg sfMouseMoveEvent_regs[] =
{
    LUA_DEFAULT_CLASS_FUNC(sfMouseMoveEvent)
    FUNCTION_TO_BIND
    { NULL, NULL }
};

LUA_REGISTER_CLASS(sfMouseMoveEvent)
