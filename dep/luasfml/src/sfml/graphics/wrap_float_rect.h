#include <lua.hpp>
#include <SFML/Graphics/Rect.hpp>

void register_sfFloatRect(lua_State * l);

#include "../../bindingHelper/lua_template.hpp"
template <>
const char* metaTableName<sf::FloatRect>();

#include "../../bindingHelper/lua_template.hpp"
template <>
const char* metaTableName<sf::FloatRect*>();
