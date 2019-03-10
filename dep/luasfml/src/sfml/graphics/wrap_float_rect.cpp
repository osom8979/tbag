#include "wrap_int_rect.h"

#include <lua.hpp>

#include "../../bindingHelper/marco.hpp"
#include "../../bindingHelper/lua_template.hpp"

#include "../system/wrap_vector2f.h"

#define TYPE_TO_BIND sf::FloatRect
#define NAME_TO_BIND sfFloatRect

#include "../../bindingHelper/macro2.hpp"

DEFAULT_CTOR_2(sf::FloatRect,sf::Vector2f,IS_POINTER,sf::Vector2f,IS_POINTER)

IMPLEMENT_LUA_CLASS(sfFloatRect,sf::FloatRect)

#define FUNCTION_TO_BIND \
    X2(__eq       , FUNCTION ,HAS_RETURN , RET_TYPE(bool)  , operator== , TYPE_TO_BIND , IS_POINTER , TYPE_TO_BIND , IS_POINTER) \
    X1(contains   , METHOD   ,HAS_RETURN , RET_TYPE(bool)  , contains   , sf::Vector2f , IS_POINTER )\
    X1(intersects , METHOD   ,HAS_RETURN , RET_TYPE(bool)  , intersects , TYPE_TO_BIND , IS_POINTER )\
    X1(setLeft    , SETTER   ,NO_RETURN  , RET_NONE        , left       , float        , NO_POINTER )\
    X1(setTop     , SETTER   ,NO_RETURN  , RET_NONE        , top        , float        , NO_POINTER )\
    X1(setWidth   , SETTER   ,NO_RETURN  , RET_NONE        , width      , float        , NO_POINTER )\
    X1(setHeight  , SETTER   ,NO_RETURN  , RET_NONE        , height     , float        , NO_POINTER )\
    X0(left       , GETTER   ,HAS_RETURN , RET_TYPE(float) , left        )\
    X0(top        , GETTER   ,HAS_RETURN , RET_TYPE(float) , top         )\
    X0(width      , GETTER   ,HAS_RETURN , RET_TYPE(float) , width       )\
    X0(height     , GETTER   ,HAS_RETURN , RET_TYPE(float) , height      )\


#define __GO_FOR_IMPLEMENTATION__
#include "../../bindingHelper/macro2.hpp"

FUNCTION_TO_BIND

#undef __GO_FOR_IMPLEMENTATION__
#include "../../bindingHelper/macro2.hpp"

luaL_Reg sfFloatRect_regs[] =
{
    LUA_DEFAULT_CLASS_FUNC(sfFloatRect)
    FUNCTION_TO_BIND
    { NULL, NULL }
};

LUA_REGISTER_CLASS(sfFloatRect)
