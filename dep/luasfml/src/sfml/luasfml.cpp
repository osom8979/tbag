#include <lua.hpp>
#include "window.h"
#include "system.h"
#include "graphics.h"

void registerSFMLModule(lua_State * l)
{
    registerSystemModule(l);
    registerWindowModule(l);
    registerGraphicsModule(l);
}

