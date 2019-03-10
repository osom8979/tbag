#include <lua.hpp>
#include <SFML/Graphics/CircleShape.hpp>


void register_sfCircleShape(lua_State * l);

#include "../../bindingHelper/lua_template.hpp"
template <>
const char* metaTableName<sf::CircleShape>();

#include "../../bindingHelper/lua_template.hpp"
template <>
const char* metaTableName<sf::CircleShape*>();
