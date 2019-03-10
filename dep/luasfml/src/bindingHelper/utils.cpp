#include "utils.hpp"

bool test_type(lua_State * l, int n,const char* name)
{
    void *p = lua_touserdata(l, n);
    if (p != NULL)
    {  /* value is a userdata? */
	if (lua_getmetatable(l, n))
	{  /* does it have a metatable? */
	    luaL_getmetatable(l, name);  /* get correct metatable */
	    if (!lua_rawequal(l, -1, -2))
	    {  /* not the same? */
		lua_pop(l, 2);  /* remove both metatables */
		return false;  /* value is a userdata with wrong metatable */
	    }
	    lua_pop(l, 2);  /* remove both metatables */
	    return true;
	}
    }
    return false;  /* value is not a userdata with a metatable */
}
