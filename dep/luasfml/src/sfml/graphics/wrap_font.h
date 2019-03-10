#include <lua.hpp>
#include <SFML/Graphics/Font.hpp>

void register_sfFont(lua_State * l);

#include "../../bindingHelper/lua_template.hpp"
template <>
const char* metaTableName<sf::Font>();

#include "../../bindingHelper/lua_template.hpp"
template <>
const char* metaTableName<sf::Font*>();
