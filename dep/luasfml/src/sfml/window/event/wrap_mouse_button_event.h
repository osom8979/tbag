#include <lua.hpp>
#include <SFML/Window/Event.hpp>


void register_sfMouseButtonEvent(lua_State * l);


#include "../../../bindingHelper/lua_template.hpp"
template <>
const char* metaTableName<sf::Event::MouseButtonEvent>();

template <>
const char* metaTableName<sf::Event::MouseButtonEvent*>();
