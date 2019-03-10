#include "wrap_circle_shape.h"

#include <lua.hpp>

#include "../../bindingHelper/marco.hpp"
#include "../../bindingHelper/lua_template.hpp"
#include "../graphics/wrap_color.h"
#include "../graphics/wrap_texture.h"
#include "../graphics/wrap_int_rect.h"
#include "../graphics/wrap_float_rect.h"
#include "../system/wrap_vector2f.h"
#include "../graphics/wrap_render_window.h"

#define TYPE_TO_BIND sf::CircleShape
#define NAME_TO_BIND sfCircleShape

#include "../../bindingHelper/macro2.hpp"
DEFAULT_CTOR_1(sf::CircleShape,float,NO_POINTER)

IMPLEMENT_LUA_CLASS(sfCircleShape,sf::CircleShape)

#define FUNCTION_TO_BIND \
    X1(setPointCount       , METHOD   , NO_RETURN , RET_NONE                , setPointCount       , unsigned int , NO_POINTER )\
    X1(setOutlineThickness , METHOD   , NO_RETURN , RET_NONE                , setOutlineThickness , float        , NO_POINTER )\
    X1(setRadius           , METHOD   , NO_RETURN , RET_NONE                , setRadius           , float        , NO_POINTER )\
    X1(setFillColor        , METHOD   , NO_RETURN , RET_NONE                , setFillColor        , sf::Color    , IS_POINTER )\
    X1(setOutlineColor     , METHOD   , NO_RETURN , RET_NONE                , setOutlineColor     , sf::Color    , IS_POINTER )\
    X1(setTextureRect      , METHOD   , NO_RETURN , RET_NONE                , setTextureRect      , sf::IntRect  , IS_POINTER )\
    X0(getPointCount       , METHOD   ,HAS_RETURN , RET_TYPE(unsigned int ) , getPointCount       )\
    X0(getOutlineThickness , METHOD   ,HAS_RETURN , RET_TYPE(float        ) , getOutlineThickness )\
    X0(getRadius           , METHOD   ,HAS_RETURN , RET_TYPE(float        ) , getRadius           )\
    X0(getFillColor        , METHOD   ,HAS_RETURN , RET_TYPE(sf::Color    ) , getFillColor        )\
    X0(getOutlineColor     , METHOD   ,HAS_RETURN , RET_TYPE(sf::Color    ) , getOutlineColor     )\
    X0(getTextureRect      , METHOD   ,HAS_RETURN , RET_TYPE(sf::IntRect  ) , getTextureRect      )\
    X0(getLocalBounds      , METHOD   ,HAS_RETURN , RET_TYPE(sf::FloatRect) , getLocalBounds      )\
    X0(getGlobalBounds     , METHOD   ,HAS_RETURN , RET_TYPE(sf::FloatRect) , getGlobalBounds     )\
    \
    \
    X0(getPosition , METHOD   ,HAS_RETURN , RET_TYPE(sf::Vector2f)  , getPosition   )\
    X0(getRotation , METHOD   ,HAS_RETURN , RET_TYPE(       float)  , getRotation   )\
    X0(getScale    , METHOD   ,HAS_RETURN , RET_TYPE(sf::Vector2f)  , getScale      )\
    X0(getOrigin   , METHOD   ,HAS_RETURN , RET_TYPE(sf::Vector2f)  , getOrigin     )\
    X1(setPosition , METHOD   , NO_RETURN , RET_NONE                , setPosition   , sf::Vector2f , IS_POINTER )\
    X1(setRotation , METHOD   , NO_RETURN , RET_NONE                , setRotation   , float        , NO_POINTER )\
    X1(setScale    , METHOD   , NO_RETURN , RET_NONE                , setScale      , sf::Vector2f , IS_POINTER )\
    X1(setOrigin   , METHOD   , NO_RETURN , RET_NONE                , setOrigin     , sf::Vector2f , IS_POINTER )\
    X1(move        , METHOD   , NO_RETURN , RET_NONE                , move          , sf::Vector2f , IS_POINTER )\
    X1(scale       , METHOD   , NO_RETURN , RET_NONE                , scale         , sf::Vector2f , IS_POINTER )\
    X1(rotate      , METHOD   , NO_RETURN , RET_NONE                , rotate        , float        , IS_POINTER )\


#define __GO_FOR_IMPLEMENTATION__
#include "../../bindingHelper/macro2.hpp"

FUNCTION_TO_BIND

#undef __GO_FOR_IMPLEMENTATION__
#include "../../bindingHelper/macro2.hpp"

luaL_Reg sfCircleShape_regs[] =
{
    LUA_DEFAULT_CLASS_FUNC(sfCircleShape)
    FUNCTION_TO_BIND
    { NULL, NULL }
};

LUA_BEGIN_REGISTER_CLASS(sfCircleShape)
register_drawableType(metaTableName<sf::CircleShape>());
LUA_END_REGISTER_CLASS
