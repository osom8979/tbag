#include <lua.hpp>
#include <SFML/Graphics/Texture.hpp>

void register_sfTexture(lua_State * l);


#include "../../bindingHelper/lua_template.hpp"
template <>
const char* metaTableName<sf::Texture>();

#include "../../bindingHelper/lua_template.hpp"
template <>
const char* metaTableName<sf::Texture*>();
