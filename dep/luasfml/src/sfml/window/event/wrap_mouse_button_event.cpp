#include "wrap_mouse_button_event.h"

#include <lua.hpp>
#include <SFML/Window/Event.hpp>

#include "../../../bindingHelper/marco.hpp"
#include "../../../bindingHelper/lua_template.hpp"

#include "../wrap_mouse.h"

#define TYPE_TO_BIND sf::Event::MouseButtonEvent
#define NAME_TO_BIND sfMouseButtonEvent

IMPLEMENT_LUA_CLASS(sfMouseButtonEvent,sf::Event::MouseButtonEvent)

#define FUNCTION_TO_BIND \
    X0(button          , GETTER   ,HAS_RETURN , RET_TYPE(               int) , button  )\
    X0(x               , GETTER   ,HAS_RETURN , RET_TYPE(               int) , x       )\
    X0(y               , GETTER   ,HAS_RETURN , RET_TYPE(               int) , y       )\

#define __GO_FOR_IMPLEMENTATION__
#include "../../../bindingHelper/macro2.hpp"

FUNCTION_TO_BIND

#undef __GO_FOR_IMPLEMENTATION__
#include "../../../bindingHelper/macro2.hpp"

luaL_Reg sfMouseButtonEvent_regs[] =
{
    LUA_DEFAULT_CLASS_FUNC(sfMouseButtonEvent)
    FUNCTION_TO_BIND
    { NULL, NULL }
};

LUA_REGISTER_CLASS(sfMouseButtonEvent)
