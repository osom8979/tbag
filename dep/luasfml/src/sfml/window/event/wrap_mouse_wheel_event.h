#include <lua.hpp>
#include <SFML/Window/Event.hpp>


void register_sfMouseWheelEvent(lua_State * l);


#include "../../../bindingHelper/lua_template.hpp"
template <>
const char* metaTableName<sf::Event::MouseWheelEvent>();

template <>
const char* metaTableName<sf::Event::MouseWheelEvent*>();
