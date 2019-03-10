#include "wrap_videomode.h"

#include <lua.hpp>

#define TYPE_TO_BIND sf::VideoMode
#define NAME_TO_BIND sfVideoMode



#include "../../bindingHelper/marco.hpp"
#include "../../bindingHelper/macro2.hpp"
#include "../../bindingHelper/lua_template.hpp"


DEFAULT_CTOR_3(sf::VideoMode,unsigned int,NO_POINTER,unsigned int,NO_POINTER,unsigned int,NO_POINTER)

IMPLEMENT_LUA_CLASS(sfVideoMode,sf::VideoMode)

#define FUNCTION_TO_BIND \
    X0(isValid          , METHOD   ,HAS_RETURN , RET_TYPE(        bool) , isValid      ) \
    X0(width            , GETTER   ,HAS_RETURN , RET_TYPE(unsigned int) , width        )\
    X0(height           , GETTER   ,HAS_RETURN , RET_TYPE(unsigned int) , height       )\
    X0(bitsPerPixel     , GETTER   ,HAS_RETURN , RET_TYPE(unsigned int) , bitsPerPixel )\
    X0(getDesktopMode   , FUNCTION ,HAS_RETURN , RET_CTOR               , sf::VideoMode::getDesktopMode) \
    X1(setWidth         , SETTER   , NO_RETURN , RET_NONE               , width        , unsigned int ,NO_POINTER)\
    X1(setHeight        , SETTER   , NO_RETURN , RET_NONE               , height       , unsigned int ,NO_POINTER)\
    X1(setBitsPerPixel  , SETTER   , NO_RETURN , RET_NONE               , bitsPerPixel , unsigned int ,NO_POINTER)\

//X3(new              , FUNCTION ,HAS_RETURN , RET_CTOR               , sf::VideoMode, unsigned int ,NO_POINTER, unsigned int ,NO_POINTER, unsigned int ,NO_POINTER) \

#define __GO_FOR_IMPLEMENTATION__
#include "../../bindingHelper/macro2.hpp"

FUNCTION_TO_BIND

#undef __GO_FOR_IMPLEMENTATION__
#include "../../bindingHelper/macro2.hpp"

luaL_Reg sfVideoMode_regs[] =
{
    LUA_DEFAULT_CLASS_FUNC(sfVideoMode)
    FUNCTION_TO_BIND
    { NULL, NULL }
};

LUA_REGISTER_CLASS(sfVideoMode)
