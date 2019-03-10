#include "wrap_render_window.h"
#include <lua.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#define TYPE_TO_BIND sf::RenderWindow
#define NAME_TO_BIND sfRenderWindow

#include "../../bindingHelper/marco.hpp"
#include "../../bindingHelper/macro2.hpp"
#include "../../bindingHelper/lua_template.hpp"
#include "../../bindingHelper/enum.hpp"

#include "../system/wrap_vector2i.h"
#include "../system/wrap_vector2u.h"
#include "../window/wrap_event.h"
#include "../window/wrap_videomode.h"
#include "../graphics/wrap_sprite.h"
#include "../graphics/wrap_text.h"
#include "../graphics/wrap_color.h"
#include "../graphics/wrap_circle_shape.h"
#include "../graphics/wrap_rectangle_shape.h"

DEFAULT_CTOR_3(sf::RenderWindow,sf::VideoMode,IS_POINTER,std::string,NO_POINTER,unsigned int,NO_POINTER)

IMPLEMENT_LUA_CLASS(sfRenderWindow,sf::RenderWindow)

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
\
X1(clear                  , METHOD   , NO_RETURN  , RET_NONE       , clear     ,sf::Color, IS_POINTER  ) \

//X1(draw                   , METHOD   , NO_RETURN  , RET_NONE       , draw   ,     sf::Drawable, IS_POINTER) \

#define __GO_FOR_IMPLEMENTATION__
#include "../../bindingHelper/macro2.hpp"

FUNCTION_TO_BIND

#undef __GO_FOR_IMPLEMENTATION__
#include "../../bindingHelper/macro2.hpp"

int sfRenderWindow_draw(lua_State* l);

luaL_Reg sfRenderWindow_regs[] =
{
    LUA_DEFAULT_CLASS_FUNC(sfRenderWindow)
    FUNCTION_TO_BIND
    {"draw" , sfRenderWindow_draw},
    { NULL, NULL }
};

LUA_REGISTER_CLASS(sfRenderWindow)

static std::vector<std::string> drawableType;


int sfRenderWindow_draw(lua_State* l)
{
    fflush(stdout);
    sf::Drawable* obj = nullptr;
    for(int i= 0; i< drawableType.size();i++)
    {
	if(test_type(l,2,drawableType[i].c_str()))
	{
	    obj = *(sf::Drawable**) luaL_checkudata(l, 2, drawableType[i].c_str());
	    break;
	}
    }
    if(obj ==nullptr)
    {
	luaL_argerror(l,2,"sfDrawable expected");
    }
    else
    {
	lua_get<sf::RenderWindow*>(l,1)->draw(*obj);
    }
    return 0;
}


void register_drawableType(const char* name)
{
    drawableType.push_back(std::string(name));
}
