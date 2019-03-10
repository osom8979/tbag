#include "wrap_sprite.h"

#include <lua.hpp>

#include "../../bindingHelper/marco.hpp"
#include "../../bindingHelper/lua_template.hpp"

#include "../graphics/wrap_render_window.h"
#include "../graphics/wrap_color.h"
#include "../graphics/wrap_texture.h"
#include "../graphics/wrap_int_rect.h"
#include "../graphics/wrap_float_rect.h"
#include "../system/wrap_vector2f.h"

#define TYPE_TO_BIND sf::Sprite
#define NAME_TO_BIND sfSprite

#include "../../bindingHelper/macro2.hpp"
DEFAULT_CTOR_1(sf::Sprite,sf::Texture,IS_POINTER)

IMPLEMENT_LUA_CLASS(sfSprite,sf::Sprite)

#define FUNCTION_TO_BIND \
    X1(setTexture      , METHOD   , NO_RETURN , RET_NONE                , setTexture     , sf::Texture , IS_POINTER )\
    X1(setColor        , METHOD   , NO_RETURN , RET_NONE                , setColor       , sf::Color   , IS_POINTER )\
    X1(setTextureRect  , METHOD   , NO_RETURN , RET_NONE                , setTextureRect , sf::IntRect , IS_POINTER )\
    X0(getLocalBounds  , METHOD   ,HAS_RETURN , RET_TYPE(sf::FloatRect) , getLocalBounds   )\
    X0(getGlobalBounds , METHOD   ,HAS_RETURN , RET_TYPE(sf::FloatRect) , getGlobalBounds   )\
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


luaL_Reg sfSprite_regs[] =
{
    LUA_DEFAULT_CLASS_FUNC(sfSprite)
    FUNCTION_TO_BIND
    { NULL, NULL }
};

LUA_BEGIN_REGISTER_CLASS(sfSprite)
register_drawableType(metaTableName<sf::Sprite>());
LUA_END_REGISTER_CLASS


