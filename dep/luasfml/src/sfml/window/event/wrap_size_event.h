#include <lua.hpp>
#include <SFML/Window/Event.hpp>


void register_sfSizeEvent(lua_State * l);


#include "../../../bindingHelper/lua_template.hpp"
template <>
const char* metaTableName<sf::Event::SizeEvent>();

template <>
const char* metaTableName<sf::Event::SizeEvent*>();
