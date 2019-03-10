#include "wrap_window.h"
#include <lua.hpp>
#include <SFML/Window/Window.hpp>
#include <SFML/Window/Event.hpp>

#define TYPE_TO_BIND sf::Window
#define NAME_TO_BIND sfWindow

#include "../../bindingHelper/marco.hpp"
#include "../../bindingHelper/macro2.hpp"
#include "../../bindingHelper/lua_template.hpp"
#include "../../bindingHelper/enum.hpp"

#include "../system/wrap_vector2i.h"
#include "../system/wrap_vector2u.h"
#include "../window/wrap_event.h"
#include "../window/wrap_videomode.h"

void register_WindowEnums(lua_State * l)
{
    add_enum_to_lua(l,"sfWindowStyle",
		    "None"       , sf::Style::None,
		    "Titlebar"   , sf::Style::Titlebar,
		    "Resize"     , sf::Style::Resize,
		    "Close"      , sf::Style::Close,
		    "Fullscreen" , sf::Style::Fullscreen,
		    "Default"    , sf::Style::Default,
		    nullptr);

}


DEFAULT_CTOR_3(sf::Window,sf::VideoMode,IS_POINTER,std::string,NO_POINTER,unsigned int,NO_POINTER)

IMPLEMENT_LUA_CLASS(sfWindow,sf::Window)

#define FUNCTION_TO_BIND \
X0(isOpen                 , METHOD   , HAS_RETURN , RET_TYPE(bool) , isOpen      ) \
X0(close                  , METHOD   , NO_RETURN  , RET_NONE       , close       ) \
X0(display                , METHOD   , NO_RETURN  , RET_NONE       , display     ) \
X0(getPosition            , METHOD   , HAS_RETURN , RET_TYPE(sf::Vector2i) , getPosition     ) \
X1(setPosition            , METHOD    ,NO_RETURN  , RET_NONE               , setPosition       , sf::Vector2i , IS_POINTER )\
X0(getSize                , METHOD   , HAS_RETURN , RET_TYPE(sf::Vector2u) , getSize         ) \
X1(setSize                , METHOD    ,NO_RETURN  , RET_NONE               , setSize                , sf::Vector2u  , IS_POINTER )\
X1(setTitle               , METHOD    ,NO_RETURN  , RET_NONE               , setTitle               , std::string   , NO_POINTER )\
X1(setVisible             , METHOD    ,NO_RETURN  , RET_NONE               , setVisible             , bool          , NO_POINTER )\
X1(setVerticalSyncEnabled , METHOD    ,NO_RETURN  , RET_NONE               , setVerticalSyncEnabled , bool          , NO_POINTER )\
X1(setMouseCursorVisible  , METHOD    ,NO_RETURN  , RET_NONE               , setMouseCursorVisible  , bool          , NO_POINTER )\
X1(setKeyRepeatEnabled    , METHOD    ,NO_RETURN  , RET_NONE               , setKeyRepeatEnabled    , bool          , NO_POINTER )\
X1(setJoystickThreshold   , METHOD    ,NO_RETURN  , RET_NONE               , setJoystickThreshold   , bool          , NO_POINTER )\
X1(setActive              , METHOD    ,NO_RETURN  , RET_NONE               , setActive              , bool          , NO_POINTER )\
X1(setFramerateLimit      , METHOD    ,NO_RETURN  , RET_NONE               , setFramerateLimit      , unsigned int  , NO_POINTER )\
X1(pollEvent              , METHOD    ,HAS_RETURN , RET_TYPE(bool)         , pollEvent              , sf::Event     , IS_POINTER )\
X1(waitEvent              , METHOD    ,HAS_RETURN , RET_TYPE(bool)         , waitEvent              , sf::Event     , IS_POINTER )\
X3(create                 , METHOD    , NO_RETURN , RET_NONE               , create                 , sf::VideoMode , IS_POINTER , std::string , NO_POINTER, unsigned int , NO_POINTER)\

//X0(getSetting             , METHOD   , HAS_RETURN , RET_TYPE(sf::ContextSettings) , getSetting         ) \
//X1(setIcon                , METHOD    ,NO_RETURN  , RET_NONE               , setIcon             , bool          , NO_POINTER )\
//X1(waitEvent                , METHOD    ,NO_RETURN  , RET_NONE               , setIcon             , bool          , NO_POINTER )\

#define __GO_FOR_IMPLEMENTATION__
#include "../../bindingHelper/macro2.hpp"

FUNCTION_TO_BIND

#undef __GO_FOR_IMPLEMENTATION__
#include "../../bindingHelper/macro2.hpp"

int sfWindow_pollEvent(lua_State* l);

luaL_Reg sfWindow_regs[] =
{
    LUA_DEFAULT_CLASS_FUNC(sfWindow)
    FUNCTION_TO_BIND
    { NULL, NULL }
};

LUA_REGISTER_CLASS(sfWindow)

