#include <lua.hpp>
#include <SFML/System/Vector2.hpp>

void register_sfVector2f(lua_State * l);

#include "../../bindingHelper/lua_template.hpp"
template <>
const char* metaTableName<sf::Vector2f>();

#include "../../bindingHelper/lua_template.hpp"
template <>
const char* metaTableName<sf::Vector2f*>();

template< >
sf::Vector2f* lua_get(lua_State * l,int i);
