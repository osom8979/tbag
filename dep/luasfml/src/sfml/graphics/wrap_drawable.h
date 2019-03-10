#include <lua.hpp>
#include <SFML/Graphics/Drawable.hpp>

void register_sfDrawable(lua_State * l);


#include "../../bindingHelper/lua_template.hpp"
template <>
const char* metaTableName<sf::Drawable>();

#include "../../bindingHelper/lua_template.hpp"
template <>
const char* metaTableName<sf::Drawable*>();
