#include <lua.hpp>
#include <SFML/Window/Window.hpp>


void register_WindowEnums(lua_State * l);


void register_sfWindow(lua_State * l);


#include "../../bindingHelper/lua_template.hpp"
template <>
const char* metaTableName<sf::Window>();

#include "../../bindingHelper/lua_template.hpp"
template <>
const char* metaTableName<sf::Window*>();
