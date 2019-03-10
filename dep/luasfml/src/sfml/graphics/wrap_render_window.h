#include <lua.hpp>
#include <SFML/Graphics/RenderWindow.hpp>



void register_sfRenderWindow(lua_State * l);

void register_drawableType(const char* name);

#include "../../bindingHelper/lua_template.hpp"
template <>
const char* metaTableName<sf::RenderWindow>();

#include "../../bindingHelper/lua_template.hpp"
template <>
const char* metaTableName<sf::RenderWindow*>();
