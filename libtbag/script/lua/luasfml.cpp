/**
 * @file   luasfml.cpp
 * @brief  luasfml class implementation.
 * @author zer0
 * @date   2019-03-10
 */

#include <libtbag/script/lua/luasfml.hpp>
#include <libtbag/log/Log.hpp>

#if defined(USE_GUI)
#include <sfml/luasfml.h>
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace script {
namespace lua    {

Err luaopen_sfml(lua_State * L)
{
#if defined(USE_GUI)
    registerSFMLModule(L);
    return Err::E_SUCCESS;
#else
    return Err::E_ENOSYS;
#endif
}

} // namespace lua
} // namespace script

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

