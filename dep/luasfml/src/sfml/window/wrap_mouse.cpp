#include "wrap_mouse.h"
#include <lua.hpp>
#include "../../bindingHelper/enum.hpp"


#include "../system/wrap_vector2i.h"
#include "../system/wrap_vector2u.h"
#include "../graphics/wrap_render_window.h"


void register_MouseEnums(lua_State * l)
{
    add_enum_to_lua(l,"sfMouseButton",
		    "Left"        , sf::Mouse::Left,
		    "Right"       , sf::Mouse::Right,
		    "Middle"      , sf::Mouse::Middle,
		    "XButton1"    , sf::Mouse::XButton1,
		    "XButton2"    , sf::Mouse::XButton2,
		    "ButtonCount" , sf::Mouse::ButtonCount,
		    nullptr);

}

template<>
sf::Mouse::Button lua_get<sf::Mouse::Button>(lua_State * l,int i)
{
    unsigned int value = lua_get<unsigned int>(l,i);
    if(value >= sf::Mouse::ButtonCount) luaL_argerror(l,i,"Not valid enum value");
    return (sf::Mouse::Button)value;
}

#include <lua.hpp>

#include "../../bindingHelper/marco.hpp"
#include "../../bindingHelper/lua_template.hpp"


#define TYPE_TO_BIND sf::Mouse
#define NAME_TO_BIND sfMouse

IMPLEMENT_LUA_CLASS(sfMouse,sf::Mouse)

#define FUNCTION_TO_BIND \
    X1(isButtonPressed        , FUNCTION ,HAS_RETURN , RET_TYPE(bool)         , sf::Mouse::isButtonPressed , sf::Mouse::Button , NO_POINTER) \
    X0(getPosition            , FUNCTION ,HAS_RETURN , RET_TYPE(sf::Vector2i) , sf::Mouse::getPosition) \
    X1(setPosition            , FUNCTION ,NO_RETURN  , RET_NONE               , sf::Mouse::setPosition , sf::Vector2i , IS_POINTER) \
    X1(getPositionRelativeto  , FUNCTION ,HAS_RETURN , RET_TYPE(sf::Vector2i) , sf::Mouse::getPosition , sf::RenderWindow,IS_POINTER) \
    X2(setPositionRelativeto  , FUNCTION ,NO_RETURN  , RET_NONE               , sf::Mouse::setPosition , sf::Vector2i , IS_POINTER, sf::RenderWindow,IS_POINTER) \


#define __GO_FOR_IMPLEMENTATION__
#include "../../bindingHelper/macro2.hpp"

FUNCTION_TO_BIND

#undef __GO_FOR_IMPLEMENTATION__
#include "../../bindingHelper/macro2.hpp"

luaL_Reg sfMouse_regs[] =
{
    LUA_DEFAULT_CLASS_FUNC(sfMouse)
    FUNCTION_TO_BIND
    { NULL, NULL }
};

LUA_REGISTER_CLASS(sfMouse)
