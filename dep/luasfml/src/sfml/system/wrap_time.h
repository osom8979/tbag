#include <lua.hpp>
#include <SFML/System/Time.hpp>

void register_sfTime(lua_State * l);

#include "../../bindingHelper/lua_template.hpp"
template <>
const char* metaTableName<sf::Time>();

#include "../../bindingHelper/lua_template.hpp"
template <>
const char* metaTableName<sf::Time*>();
