#include "wrap_texture.h"

#include <lua.hpp>

#include "../../bindingHelper/marco.hpp"
#include "../../bindingHelper/lua_template.hpp"

#include "../system/wrap_vector2u.h"

#define TYPE_TO_BIND sf::Texture
#define NAME_TO_BIND sfTexture

IMPLEMENT_LUA_CLASS(sfTexture,sf::Texture)

#define FUNCTION_TO_BIND \
X0(getMaximumSize , FUNCTION ,HAS_RETURN  , RET_TYPE(unsigned int) , sf::Texture::getMaximumSize     )\
X0(getSize        , METHOD   ,HAS_RETURN  , RET_TYPE(sf::Vector2u) , getSize     )\
X0(isSmooth       , METHOD   ,HAS_RETURN  , RET_TYPE(bool)         , isSmooth    )\
X0(isRepeated     , METHOD   ,HAS_RETURN  , RET_TYPE(bool)         , isRepeated  )\
X1(setSmooth      , METHOD   ,NO_RETURN   , RET_NONE               , setSmooth    , bool          , NO_POINTER )\
X1(setRepeated    , METHOD   ,NO_RETURN   , RET_NONE               , setRepeated  , bool          , NO_POINTER )\
X1(loadFromFile   , METHOD   ,HAS_RETURN  , RET_TYPE(bool)         , loadFromFile , std::string   , NO_POINTER )\
X2(create         , METHOD   ,HAS_RETURN  , RET_TYPE(bool)         , create       , unsigned int  , NO_POINTER , unsigned int  , NO_POINTER )\

#define __GO_FOR_IMPLEMENTATION__
#include "../../bindingHelper/macro2.hpp"

FUNCTION_TO_BIND

#undef __GO_FOR_IMPLEMENTATION__
#include "../../bindingHelper/macro2.hpp"

luaL_Reg sfTexture_regs[] =
{
    LUA_DEFAULT_CLASS_FUNC(sfTexture)
    FUNCTION_TO_BIND
    { NULL, NULL }
};

LUA_REGISTER_CLASS(sfTexture)
