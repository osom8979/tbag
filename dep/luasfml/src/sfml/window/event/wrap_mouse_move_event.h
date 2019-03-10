#include <lua.hpp>
#include <SFML/Window/Event.hpp>


void register_sfMouseMoveEvent(lua_State * l);


#include "../../../bindingHelper/lua_template.hpp"
template <>
const char* metaTableName<sf::Event::MouseMoveEvent>();

template <>
const char* metaTableName<sf::Event::MouseMoveEvent*>();
