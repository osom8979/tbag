#include "enum.hpp"
#include <iostream>
#include <sstream>
#include<lua.hpp>

bool add_enum_to_lua(lua_State* L, const char* tname, ...)
{
    // NOTE: Here's the Lua code we're building and executing to define the
    //       enum.
    //
    // <tname> = setmetatable( {}, {
    //      __index = {
    //          <name1> = {
    //              value = <value1>,
    //              type = \"<tname>\"
    //          },
    //          ...
    //      },
    //      __newindex = function(table, key, value)
    //          error(\"Attempt to modify read-only table\")
    //      end,
    //      __metatable = false
    // });

    va_list args;
    std::stringstream code;
    char* ename;
    int evalue;

    code << tname << " = setmetatable({}, {";
    code << "__index = {";

    // Iterate over the variadic arguments adding the enum values.
    va_start(args, tname);
    while ((ename = va_arg(args, char*)) != 0)
    {
	evalue = va_arg(args, int);
	code << ename << "=" << evalue << ", ";
    }
    va_end(args);

    code << "},";
    code << "__newindex = function(table, key, value) error(\"Attempt to modify read-only table\") end,";
    code << "__metatable = false});";

    // Execute lua code
    if ( luaL_loadbuffer(L, code.str().c_str(), code.str().length(),0) || lua_pcall(L, 0, 0, 0) )
    {
	fprintf(stderr, "%s\n", lua_tostring(L, -1));
	lua_pop(L, 1);
	return false;
    }
    return true;
}
