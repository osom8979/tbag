#include "wrap_color.h"

#include <lua.hpp>

#include "../../bindingHelper/marco.hpp"
#include "../../bindingHelper/lua_template.hpp"

#define TYPE_TO_BIND sf::Color
#define NAME_TO_BIND sfColor

#include "../../bindingHelper/macro2.hpp"

DEFAULT_CTOR_3(sf::Color,unsigned char,NO_POINTER,unsigned char,NO_POINTER,unsigned char,NO_POINTER)

IMPLEMENT_LUA_CLASS(sfColor,sf::Color)

#define FUNCTION_TO_BIND \
    X1(setR , SETTER ,NO_RETURN  , RET_NONE                , r  , unsigned char , NO_POINTER )\
    X1(setG , SETTER ,NO_RETURN  , RET_NONE                , g  , unsigned char , NO_POINTER )\
    X1(setB , SETTER ,NO_RETURN  , RET_NONE                , b  , unsigned char , NO_POINTER )\
    X1(setA , SETTER ,NO_RETURN  , RET_NONE                , a  , unsigned char , NO_POINTER )\
    X0(r    , GETTER ,HAS_RETURN , RET_TYPE(unsigned char) , r  )\
    X0(g    , GETTER ,HAS_RETURN , RET_TYPE(unsigned char) , g  )\
    X0(b    , GETTER ,HAS_RETURN , RET_TYPE(unsigned char) , b  )\
    X0(a    , GETTER ,HAS_RETURN , RET_TYPE(unsigned char) , a  )\
    X2(__mul          , FUNCTION ,HAS_RETURN , RET_CTOR        , operator*        , TYPE_TO_BIND , IS_POINTER , TYPE_TO_BIND , IS_POINTER) \
    X2(__add          , FUNCTION ,HAS_RETURN , RET_CTOR        , operator+        , TYPE_TO_BIND , IS_POINTER , TYPE_TO_BIND , IS_POINTER) \
    X2(__eq           , FUNCTION ,HAS_RETURN , RET_TYPE(bool)  , operator==       , TYPE_TO_BIND , IS_POINTER , TYPE_TO_BIND , IS_POINTER) \


#define __GO_FOR_IMPLEMENTATION__
#include "../../bindingHelper/macro2.hpp"

FUNCTION_TO_BIND

#undef __GO_FOR_IMPLEMENTATION__
#include "../../bindingHelper/macro2.hpp"

luaL_Reg sfColor_regs[] =
{
    LUA_DEFAULT_CLASS_FUNC(sfColor)
    FUNCTION_TO_BIND
    { NULL, NULL }
};

LUA_REGISTER_CLASS(sfColor)
