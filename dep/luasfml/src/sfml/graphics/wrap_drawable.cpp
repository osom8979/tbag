#include "wrap_drawable.h"

#include <lua.hpp>

#include "../../bindingHelper/marco.hpp"
#include "../../bindingHelper/lua_template.hpp"

template <>
sf::Drawable* newDefault(lua_State * l)
{
    luaL_error(l,"cannot instanciate sfDrawable directly");
    return nullptr;
}

#define TYPE_TO_BIND sf::Drawable
#define NAME_TO_BIND sfDrawable

IMPLEMENT_LUA_CLASS(sfDrawable,sf::Drawable)

#define FUNCTION_TO_BIND \
    // define your function here

#define __GO_FOR_IMPLEMENTATION__
#include "../../bindingHelper/macro2.hpp"

FUNCTION_TO_BIND

#undef __GO_FOR_IMPLEMENTATION__
#include "../../bindingHelper/macro2.hpp"

luaL_Reg sfDrawable_regs[] =
{
    LUA_DEFAULT_CLASS_FUNC(sfDrawable)
    FUNCTION_TO_BIND
    { NULL, NULL }
};

LUA_REGISTER_CLASS(sfDrawable)
