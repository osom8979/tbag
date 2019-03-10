#include <lua.hpp>
#include <SFML/Graphics/Color.hpp>

void register_sfColor(lua_State * l);

#include "../../bindingHelper/lua_template.hpp"
template <>
const char* metaTableName<sf::Color>();

#include "../../bindingHelper/lua_template.hpp"
template <>
const char* metaTableName<sf::Color*>();
