#include <lua.hpp>
#include <SFML/Graphics/Text.hpp>

void register_TextEnums(lua_State * l);

void register_sfText(lua_State * l);

#include "../../bindingHelper/lua_template.hpp"
template <>
const char* metaTableName<sf::Text>();

#include "../../bindingHelper/lua_template.hpp"
template <>
const char* metaTableName<sf::Text*>();
