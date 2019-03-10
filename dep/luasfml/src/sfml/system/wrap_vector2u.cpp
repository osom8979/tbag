#include "wrap_vector2u.h"
#include <lua.hpp>
#include <SFML/System/Vector2.hpp>

#include <lua.hpp>

#include "../../bindingHelper/marco.hpp"
#include "../../bindingHelper/lua_template.hpp"
#include "../system/wrap_vector2i.h"
#include "../system/wrap_vector2f.h"


#define TYPE_TO_BIND sf::Vector2u
#define NAME_TO_BIND sfVector2u

template< >
sf::Vector2u* lua_get(lua_State * l,int i)
{

    if(test_type<sf::Vector2i*>(l,i))
    {
	sf::Vector2i* tmp = check_pointertype<sf::Vector2i*>(l,i);
	copy_constructor<sf::Vector2u>(l,sf::Vector2u(tmp->x,tmp->y));
	lua_replace(l,i);
    }
    if(test_type<sf::Vector2f*>(l,i))
    {
	sf::Vector2f* tmp = check_pointertype<sf::Vector2f*>(l,i);
	copy_constructor<sf::Vector2u>(l,sf::Vector2u(tmp->x,tmp->y));
	lua_replace(l,i);
    }
    if(lua_isnumber(l,i) && lua_isnumber(l,i+1))
    {
	copy_constructor<sf::Vector2u>(l,sf::Vector2u(lua_get<unsigned int>(l,i),lua_get<unsigned int>(l,i+1)));
	lua_replace(l,i);
	lua_remove(l,i+1);
    }
    return check_pointertype<sf::Vector2u*>(l,i);
}


#include "../../bindingHelper/macro2.hpp"


DEFAULT_CTOR_2(sf::Vector2u,unsigned int,NO_POINTER,unsigned int,NO_POINTER)

IMPLEMENT_LUA_CLASS(sfVector2u,sf::Vector2u)

#define FUNCTION_TO_BIND \
    X2(__mul          , FUNCTION ,HAS_RETURN , RET_CTOR        , operator*        , TYPE_TO_BIND , IS_POINTER , unsigned int        , NO_POINTER) \
    X2(__div          , FUNCTION ,HAS_RETURN , RET_CTOR        , operator/        , TYPE_TO_BIND , IS_POINTER , unsigned int        , NO_POINTER) \
    X2(__add          , FUNCTION ,HAS_RETURN , RET_CTOR        , operator+        , TYPE_TO_BIND , IS_POINTER , TYPE_TO_BIND , IS_POINTER) \
    X2(__sub          , FUNCTION ,HAS_RETURN , RET_CTOR        , operator-        , TYPE_TO_BIND , IS_POINTER , TYPE_TO_BIND , IS_POINTER) \
    X2(__eq           , FUNCTION ,HAS_RETURN , RET_TYPE(bool)  , operator==       , TYPE_TO_BIND , IS_POINTER , TYPE_TO_BIND , IS_POINTER) \
    X1(setX           , SETTER   ,NO_RETURN  , RET_NONE        , x                , unsigned int        , NO_POINTER )\
    X1(setY           , SETTER   ,NO_RETURN  , RET_NONE        , y                , unsigned int        , NO_POINTER )\
    X0(x              , GETTER   ,HAS_RETURN , RET_TYPE(unsigned int) , x                )\
    X0(y              , GETTER   ,HAS_RETURN , RET_TYPE(unsigned int) , y                )\

#define __GO_FOR_IMPLEMENTATION__
#include "../../bindingHelper/macro2.hpp"

FUNCTION_TO_BIND

#undef __GO_FOR_IMPLEMENTATION__
#include "../../bindingHelper/macro2.hpp"

luaL_Reg sfVector2u_regs[] =
{
    LUA_DEFAULT_CLASS_FUNC(sfVector2u)
    FUNCTION_TO_BIND
    { NULL, NULL }
};

LUA_REGISTER_CLASS(sfVector2u)


