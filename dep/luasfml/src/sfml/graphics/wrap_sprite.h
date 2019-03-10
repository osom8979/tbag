#include <lua.hpp>
#include <SFML/Graphics/Sprite.hpp>

void register_sfSprite(lua_State * l);


#include "../../bindingHelper/lua_template.hpp"
template <>
const char* metaTableName<sf::Sprite>();

#include "../../bindingHelper/lua_template.hpp"
template <>
const char* metaTableName<sf::Sprite*>();
