#include <lua.hpp>
#include <SFML/System/Vector3.hpp>

void register_sfVector3i(lua_State * l);

#include "../../bindingHelper/lua_template.hpp"
template <>
const char* metaTableName<sf::Vector3i>();

#include "../../bindingHelper/lua_template.hpp"
template <>
const char* metaTableName<sf::Vector3i*>();
