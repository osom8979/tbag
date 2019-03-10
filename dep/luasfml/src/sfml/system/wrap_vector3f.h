#include <lua.hpp>
#include <SFML/System/Vector3.hpp>

void register_sfVector3f(lua_State * l);

#include "../../bindingHelper/lua_template.hpp"
template <>
const char* metaTableName<sf::Vector3f>();

#include "../../bindingHelper/lua_template.hpp"
template <>
const char* metaTableName<sf::Vector3f*>();
