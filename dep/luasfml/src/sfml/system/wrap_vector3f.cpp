#include "wrap_vector2f.h"
#include "wrap_vector2i.h"
#include "wrap_vector2u.h"
#include <lua.hpp>
#include <SFML/System/Vector3.hpp>

#include "../../bindingHelper/marco.hpp"
#include "../../bindingHelper/lua_template.hpp"


#define TYPE_TO_BIND sf::Vector3f
#define NAME_TO_BIND sfVector3f


#include "../../bindingHelper/macro2.hpp"


DEFAULT_CTOR_3(sf::Vector3f,float,NO_POINTER,float,NO_POINTER,float,NO_POINTER)

    IMPLEMENT_LUA_CLASS(sfVector3f,sf::Vector3f)

#define FUNCTION_TO_BIND \
    X2(__mul          , FUNCTION ,HAS_RETURN , RET_CTOR        , operator*        , TYPE_TO_BIND , IS_POINTER , float        , NO_POINTER) \
    X2(__div          , FUNCTION ,HAS_RETURN , RET_CTOR        , operator/        , TYPE_TO_BIND , IS_POINTER , float        , NO_POINTER) \
    X2(__add          , FUNCTION ,HAS_RETURN , RET_CTOR        , operator+        , TYPE_TO_BIND , IS_POINTER , TYPE_TO_BIND , IS_POINTER) \
    X2(__sub          , FUNCTION ,HAS_RETURN , RET_CTOR        , operator-        , TYPE_TO_BIND , IS_POINTER , TYPE_TO_BIND , IS_POINTER) \
    X2(__eq           , FUNCTION ,HAS_RETURN , RET_TYPE(bool)  , operator==       , TYPE_TO_BIND , IS_POINTER , TYPE_TO_BIND , IS_POINTER) \
    X1(setX           , SETTER   ,NO_RETURN  , RET_NONE        , x                , float        , NO_POINTER )\
    X1(setY           , SETTER   ,NO_RETURN  , RET_NONE        , y                , float        , NO_POINTER )\
    X1(setZ           , SETTER   ,NO_RETURN  , RET_NONE        , z                , float        , NO_POINTER )\
    X0(z              , GETTER   ,HAS_RETURN , RET_TYPE(float) , z                )\
    X0(x              , GETTER   ,HAS_RETURN , RET_TYPE(float) , x                )\
    X0(y              , GETTER   ,HAS_RETURN , RET_TYPE(float) , y                )\


#define __GO_FOR_IMPLEMENTATION__
#include "../../bindingHelper/macro2.hpp"

    FUNCTION_TO_BIND

#undef __GO_FOR_IMPLEMENTATION__
#include "../../bindingHelper/macro2.hpp"

    luaL_Reg sfVector3f_regs[] =
    {
	LUA_DEFAULT_CLASS_FUNC(sfVector3f)
	FUNCTION_TO_BIND
	{ NULL, NULL }
    };

    LUA_REGISTER_CLASS(sfVector3f)


