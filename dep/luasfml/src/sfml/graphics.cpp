#include <lua.hpp>
#include "graphics/wrap_render_window.h"
#include "graphics/wrap_drawable.h"
#include "graphics/wrap_sprite.h"
#include "graphics/wrap_texture.h"
#include "graphics/wrap_color.h"
#include "graphics/wrap_float_rect.h"
#include "graphics/wrap_int_rect.h"
#include "graphics/wrap_font.h"
#include "graphics/wrap_text.h"
#include "graphics/wrap_circle_shape.h"
#include "graphics/wrap_rectangle_shape.h"

void registerGraphicsModule(lua_State * l)
{
   register_TextEnums(l);

   register_sfRenderWindow(l);
   register_sfDrawable(l);
   register_sfSprite(l);
   register_sfTexture(l);
   register_sfColor(l);
   register_sfFloatRect(l);
   register_sfIntRect(l);
   register_sfFont(l);
   register_sfText(l);
   register_sfCircleShape(l);
   register_sfRectangleShape(l);
}

