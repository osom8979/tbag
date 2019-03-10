#include <lua.hpp>
#include <SFML/System/Vector2.hpp>

void register_sfVector2i(lua_State * l);

#include "../../bindingHelper/lua_template.hpp"
template <>
const char* metaTableName<sf::Vector2i>();

#include "../../bindingHelper/lua_template.hpp"
template <>
const char* metaTableName<sf::Vector2i*>();

template< >
sf::Vector2i* lua_get(lua_State * l,int i);
