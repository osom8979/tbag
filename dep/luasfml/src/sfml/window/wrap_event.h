#include <lua.hpp>
#include <SFML/Window/Event.hpp>

void register_EventEnums(lua_State * l);


void register_sfEvent(lua_State * l);


#include "../../bindingHelper/lua_template.hpp"
template <>
const char* metaTableName<sf::Event>();

#include "../../bindingHelper/lua_template.hpp"
template <>
const char* metaTableName<sf::Event*>();
