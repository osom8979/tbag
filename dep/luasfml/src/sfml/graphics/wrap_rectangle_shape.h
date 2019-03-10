#include <lua.hpp>
#include <SFML/Graphics/RectangleShape.hpp>


void register_sfRectangleShape(lua_State * l);

#include "../../bindingHelper/lua_template.hpp"
template <>
const char* metaTableName<sf::RectangleShape>();

#include "../../bindingHelper/lua_template.hpp"
template <>
const char* metaTableName<sf::RectangleShape*>();
