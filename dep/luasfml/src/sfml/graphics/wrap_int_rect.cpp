#include "wrap_int_rect.h"

#include <lua.hpp>

#include "../../bindingHelper/marco.hpp"
#include "../../bindingHelper/lua_template.hpp"

#include "../system/wrap_vector2i.h"

#define TYPE_TO_BIND sf::IntRect
#define NAME_TO_BIND sfIntRect

#include "../../bindingHelper/macro2.hpp"

DEFAULT_CTOR_2(sf::IntRect,sf::Vector2i,IS_POINTER,sf::Vector2i,IS_POINTER)

IMPLEMENT_LUA_CLASS(sfIntRect,sf::IntRect)

#define FUNCTION_TO_BIND \
    X2(__eq       , FUNCTION ,HAS_RETURN , RET_TYPE(bool)  , operator== , TYPE_TO_BIND , IS_POINTER , TYPE_TO_BIND , IS_POINTER) \
    X1(contains   , METHOD   ,HAS_RETURN , RET_TYPE(bool)  , contains   , sf::Vector2i , IS_POINTER )\
    X1(intersects , METHOD   ,HAS_RETURN , RET_TYPE(bool)  , intersects , TYPE_TO_BIND , IS_POINTER )\
    X1(setLeft    , SETTER   ,NO_RETURN  , RET_NONE        , left       , int          , NO_POINTER )\
    X1(setTop     , SETTER   ,NO_RETURN  , RET_NONE        , top        , int          , NO_POINTER )\
    X1(setWidth   , SETTER   ,NO_RETURN  , RET_NONE        , width      , int          , NO_POINTER )\
    X1(setHeight  , SETTER   ,NO_RETURN  , RET_NONE        , height     , int          , NO_POINTER )\
    X0(left       , GETTER   ,HAS_RETURN , RET_TYPE(int)   , left        )\
    X0(top        , GETTER   ,HAS_RETURN , RET_TYPE(int)   , top         )\
    X0(width      , GETTER   ,HAS_RETURN , RET_TYPE(int)   , width       )\
    X0(height     , GETTER   ,HAS_RETURN , RET_TYPE(int)   , height      )\


#define __GO_FOR_IMPLEMENTATION__
#include "../../bindingHelper/macro2.hpp"

FUNCTION_TO_BIND

#undef __GO_FOR_IMPLEMENTATION__
#include "../../bindingHelper/macro2.hpp"

luaL_Reg sfIntRect_regs[] =
{
    LUA_DEFAULT_CLASS_FUNC(sfIntRect)
    FUNCTION_TO_BIND
    { NULL, NULL }
};

LUA_REGISTER_CLASS(sfIntRect)
