#include "wrap_key_event.h"
#include <SFML/Window/Event.hpp>

#include "../wrap_keyboard.h"

#include <lua.hpp>

#include "../../../bindingHelper/marco.hpp"
#include "../../../bindingHelper/lua_template.hpp"


#define TYPE_TO_BIND sf::Event::KeyEvent
#define NAME_TO_BIND sfKeyEvent

IMPLEMENT_LUA_CLASS(sfKeyEvent,sf::Event::KeyEvent)

#define FUNCTION_TO_BIND \
X0(code     , GETTER   ,HAS_RETURN , RET_TYPE(              int) , code      )\
X0(alt      , GETTER   ,HAS_RETURN , RET_TYPE(              bool) , alt       )\
X0(control  , GETTER   ,HAS_RETURN , RET_TYPE(              bool) , control   )\
X0(shift    , GETTER   ,HAS_RETURN , RET_TYPE(              bool) , shift     )\
X0(system   , GETTER   ,HAS_RETURN , RET_TYPE(              bool) , system    )\

#define __GO_FOR_IMPLEMENTATION__
#include "../../../bindingHelper/macro2.hpp"

FUNCTION_TO_BIND

#undef __GO_FOR_IMPLEMENTATION__
#include "../../../bindingHelper/macro2.hpp"

luaL_Reg sfKeyEvent_regs[] =
{
    LUA_DEFAULT_CLASS_FUNC(sfKeyEvent)
    FUNCTION_TO_BIND
    { NULL, NULL }
};

LUA_REGISTER_CLASS(sfKeyEvent)

