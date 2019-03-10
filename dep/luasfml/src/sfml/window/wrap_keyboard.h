#include <lua.hpp>
#include <SFML/Window/Keyboard.hpp>

void register_KeyBoardEnums(lua_State * l);


void register_sfKeyboard(lua_State * l);


#include "../../bindingHelper/lua_template.hpp"
template <>
const char* metaTableName<sf::Keyboard>();

template <>
const char* metaTableName<sf::Keyboard*>();

template<>
sf::Keyboard::Key lua_get<sf::Keyboard::Key>(lua_State * l,int i);
