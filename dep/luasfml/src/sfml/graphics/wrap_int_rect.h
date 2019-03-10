#include <lua.hpp>
#include <SFML/Graphics/Rect.hpp>

void register_sfIntRect(lua_State * l);

#include "../../bindingHelper/lua_template.hpp"
template <>
const char* metaTableName<sf::IntRect>();

#include "../../bindingHelper/lua_template.hpp"
template <>
const char* metaTableName<sf::IntRect*>();
