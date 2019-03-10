#include <lua.hpp>
#include "system/wrap_clock.h"
#include "system/wrap_time.h"
#include "system/wrap_vector2i.h"
#include "system/wrap_vector2f.h"
#include "system/wrap_vector2u.h"
#include "system/wrap_vector3i.h"
#include "system/wrap_vector3f.h"

void registerSystemModule(lua_State * l)
{
   register_sfClock(l);
   register_sfTime(l);
   register_sfVector2i(l);
   register_sfVector2u(l);
   register_sfVector2f(l);
   register_sfVector3i(l);
   register_sfVector3f(l);

}

