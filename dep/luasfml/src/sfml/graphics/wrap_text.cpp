#include "wrap_text.h"
#include "wrap_font.h"

#include <lua.hpp>

#include <string>
#include "../../bindingHelper/marco.hpp"
#include "../../bindingHelper/lua_template.hpp"
#include "../../bindingHelper/enum.hpp"

void register_TextEnums(lua_State * l)
{
    add_enum_to_lua(l,"sfTextStyle",
		    "Regular"    , sf::Text::Regular,
		    "Bold"       , sf::Text::Bold,
		    "Italic"     , sf::Text::Italic,
		    "Underlined" , sf::Text::Underlined,
		    nullptr);

}


#define TYPE_TO_BIND sf::Text
#define NAME_TO_BIND sfText

#include "../../bindingHelper/macro2.hpp"
#include "../graphics/wrap_render_window.h"
#include "../graphics/wrap_font.h"
#include "../graphics/wrap_color.h"
#include "../system/wrap_vector2f.h"
#include "../graphics/wrap_float_rect.h"

DEFAULT_CTOR_3(sf::Text,std::string,NO_POINTER,sf::Font,IS_POINTER,unsigned int,NO_POINTER)

IMPLEMENT_LUA_CLASS(sfText,sf::Text)

#define FUNCTION_TO_BIND \
    X1(setStyle         , METHOD   , NO_RETURN , RET_NONE , setStyle         , unsigned int , NO_POINTER )\
    X1(setFont          , METHOD   , NO_RETURN , RET_NONE , setFont          , sf::Font     , IS_POINTER )\
    X1(setColor         , METHOD   , NO_RETURN , RET_NONE , setColor         , sf::Color    , IS_POINTER )\
    X1(setString        , METHOD   , NO_RETURN , RET_NONE , setString        , std::string  , NO_POINTER )\
    X1(setCharacterSize , METHOD   , NO_RETURN , RET_NONE , setCharacterSize , unsigned int , NO_POINTER )\
    X0(getStyle         , METHOD   ,HAS_RETURN , RET_TYPE(unsigned int) , getStyle          )\
    X0(getColor         , METHOD   ,HAS_RETURN , RET_TYPE(sf::Color   ) , getColor          )\
    X0(getString        , METHOD   ,HAS_RETURN , RET_TYPE(std::string ) , getString         )\
    X0(getCharacterSize , METHOD   , NO_RETURN , RET_TYPE(unsigned int) , getCharacterSize  )\
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
    X1(rotate      , METHOD   , NO_RETURN , RET_NONE                , rotate        , float        , NO_POINTER )\


#define __GO_FOR_IMPLEMENTATION__
#include "../../bindingHelper/macro2.hpp"

FUNCTION_TO_BIND

#undef __GO_FOR_IMPLEMENTATION__
#include "../../bindingHelper/macro2.hpp"

luaL_Reg sfText_regs[] =
{
    LUA_DEFAULT_CLASS_FUNC(sfText)
    FUNCTION_TO_BIND
    { NULL, NULL }
};

LUA_BEGIN_REGISTER_CLASS(sfText)
register_drawableType(metaTableName<sf::Text>());
LUA_END_REGISTER_CLASS
