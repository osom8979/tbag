#include "wrap_time.h"
#include <lua.hpp>
#include <SFML/System/Time.hpp>

#include "../../bindingHelper/marco.hpp"
#include "../../bindingHelper/lua_template.hpp"

template <>
sf::Time* newDefault<sf::Time>(lua_State* l)
{
    return new sf::Time(sf::Time::Zero);
}

#define TYPE_TO_BIND sf::Time
#define NAME_TO_BIND sfTime

IMPLEMENT_LUA_CLASS(sfTime,sf::Time)
//IMPLEMENT_LUA_COPY_CTOR(sfTime,sf::Time)

#define FUNCTION_TO_BIND \
    X0(asSeconds      , METHOD   ,HAS_RETURN , RET_TYPE(float)   , asSeconds      )                  \
    X0(asMilliseconds , METHOD   ,HAS_RETURN , RET_TYPE(int)   , asMilliseconds )                  \
    X0(asMicroseconds , METHOD   ,HAS_RETURN , RET_TYPE(int)   , asMicroseconds )                  \
    X1(seconds        , FUNCTION ,HAS_RETURN , RET_CTOR        , sf::seconds      , float        , NO_POINTER) \
    X1(milliseconds   , FUNCTION ,HAS_RETURN , RET_CTOR        , sf::milliseconds , int          , NO_POINTER) \
    X1(microseconds   , FUNCTION ,HAS_RETURN , RET_CTOR        , sf::microseconds , int          , NO_POINTER) \
    X2(__mul          , FUNCTION ,HAS_RETURN , RET_CTOR        , operator*        , TYPE_TO_BIND , IS_POINTER , float        , NO_POINTER) \
    X2(__div          , FUNCTION ,HAS_RETURN , RET_CTOR        , operator/        , TYPE_TO_BIND , IS_POINTER , float        , NO_POINTER) \
    X2(__add          , FUNCTION ,HAS_RETURN , RET_CTOR        , operator+        , TYPE_TO_BIND , IS_POINTER , TYPE_TO_BIND , IS_POINTER) \
    X2(__sub          , FUNCTION ,HAS_RETURN , RET_CTOR        , operator-        , TYPE_TO_BIND , IS_POINTER , TYPE_TO_BIND , IS_POINTER) \
    X2(__eq           , FUNCTION ,HAS_RETURN , RET_TYPE(bool)  , operator==       , TYPE_TO_BIND , IS_POINTER , TYPE_TO_BIND , IS_POINTER) \
    X2(__lt           , FUNCTION ,HAS_RETURN , RET_TYPE(bool)  , operator<        , TYPE_TO_BIND , IS_POINTER , TYPE_TO_BIND , IS_POINTER) \
    X2(__le           , FUNCTION ,HAS_RETURN , RET_TYPE(bool)  , operator<=       , TYPE_TO_BIND , IS_POINTER , TYPE_TO_BIND , IS_POINTER) \

#define __GO_FOR_IMPLEMENTATION__
#include "../../bindingHelper/macro2.hpp"

FUNCTION_TO_BIND

#undef __GO_FOR_IMPLEMENTATION__
#include "../../bindingHelper/macro2.hpp"

luaL_Reg sfTime_regs[] =
{
    LUA_DEFAULT_CLASS_FUNC(sfTime)
    FUNCTION_TO_BIND
    { NULL, NULL }
};

LUA_REGISTER_CLASS(sfTime)
