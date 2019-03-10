#include <lua.hpp>
#include <SFML/Window/VideoMode.hpp>



void register_sfVideoMode(lua_State * l);


#include "../../bindingHelper/lua_template.hpp"
template <>
const char* metaTableName<sf::VideoMode>();

#include "../../bindingHelper/lua_template.hpp"
template <>
const char* metaTableName<sf::VideoMode*>();
