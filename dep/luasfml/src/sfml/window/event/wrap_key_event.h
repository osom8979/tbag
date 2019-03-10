#include <lua.hpp>
#include <SFML/Window/Event.hpp>


void register_sfKeyEvent(lua_State * l);


#include "../../../bindingHelper/lua_template.hpp"
template <>
const char* metaTableName<sf::Event::KeyEvent>();

template <>
const char* metaTableName<sf::Event::KeyEvent*>();
