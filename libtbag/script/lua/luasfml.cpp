/**
 * @file   luasfml.cpp
 * @brief  luasfml class implementation.
 * @author zer0
 * @date   2019-03-10
 */

#include <libtbag/script/lua/luasfml.hpp>
#include <libtbag/log/Log.hpp>

#include <sfml/luasfml.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace script {
namespace lua    {

void luaopen_sfml(lua_State * L)
{
    registerSFMLModule(L);
}

} // namespace lua
} // namespace script

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

