/**
 * @file   RayGuiBypass.hpp
 * @brief  RayGuiBypass class prototype.
 * @author zer0
 * @date   2019-04-20
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_RAY_RAYGUIBYPASS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_RAY_RAYGUIBYPASS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/ray/RayBypass.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace ray {

TBAG_API void GuiEnable();
TBAG_API void GuiDisable();
TBAG_API void GuiLock();
TBAG_API void GuiUnlock();
TBAG_API void GuiState(int state);
TBAG_API void GuiFont(Font font);
TBAG_API void GuiFade(float alpha);

TBAG_API void GuiSetStyle(int control, int property, int value);
TBAG_API int GuiGetStyle(int control, int property);

TBAG_API bool GuiWindowBox(Rectangle bounds, char const * text);
TBAG_API void GuiGroupBox(Rectangle bounds, char const * text);
TBAG_API void GuiLine(Rectangle bounds, char const * text);
TBAG_API void GuiPanel(Rectangle bounds);
TBAG_API Rectangle GuiScrollPanel(Rectangle bounds, Rectangle content, Vector2 * scroll);

TBAG_API void GuiLabel(Rectangle bounds, char const * text);
TBAG_API bool GuiButton(Rectangle bounds, char const * text);
TBAG_API bool GuiLabelButton(Rectangle bounds, char const * text);
TBAG_API bool GuiImageButton(Rectangle bounds, Texture2D texture);
TBAG_API bool GuiImageButtonEx(Rectangle bounds, Texture2D texture, Rectangle tex_source, char const * text);
TBAG_API bool GuiToggle(Rectangle bounds, char const * text, bool active);
TBAG_API int GuiToggleGroup(Rectangle bounds, char const * text, int active);
TBAG_API bool GuiCheckBox(Rectangle bounds, char const * text, bool checked);
TBAG_API int GuiComboBox(Rectangle bounds, char const * text, int active);
TBAG_API bool GuiDropdownBox(Rectangle bounds, char const * text, int *active, bool edit_mode);
TBAG_API bool GuiSpinner(Rectangle bounds, int * value, int min_value, int max_value, bool edit_mode);
TBAG_API bool GuiValueBox(Rectangle bounds, int * value, int min_value, int max_value, bool edit_mode);
TBAG_API bool GuiTextBox(Rectangle bounds, char * text, int text_size, bool edit_mode);
TBAG_API bool GuiTextBoxMulti(Rectangle bounds, char * text, int text_size, bool edit_mode);
TBAG_API float GuiSlider(Rectangle bounds, char const * text, float value,
                         float min_value, float max_value, bool show_value);
TBAG_API float GuiSliderBar(Rectangle bounds, char const * text, float value,
                            float min_value, float max_value, bool show_value);
TBAG_API float GuiProgressBar(Rectangle bounds, char const * text, float value,
                              float min_value, float max_value, bool show_value);
TBAG_API void GuiStatusBar(Rectangle bounds, char const * text);
TBAG_API void GuiDummyRec(Rectangle bounds, char const * text);
TBAG_API int GuiScrollBar(Rectangle bounds, int value, int min_value, int max_value);

TBAG_API bool GuiListView(Rectangle bounds, char const * text, int * active, int * scroll_index, bool edit_mode);
TBAG_API bool GuiListViewEx(Rectangle bounds, char const ** text, int count, int * enabled, int * active, int * focus,
                            int * scroll_index, bool edit_mode);
TBAG_API int GuiMessageBox(Rectangle bounds, char const * window_title, char const * message, char const * buttons);
TBAG_API Color GuiColorPicker(Rectangle bounds, Color color);
TBAG_API Vector2 GuiGrid(Rectangle bounds, float spacing, int subdivs);

TBAG_API void GuiLoadStyle(char const * file_name);
TBAG_API void GuiLoadStyleProps(const int * props, int count);
TBAG_API void GuiLoadStyleDefault();
TBAG_API void GuiUpdateStyleComplete();

} // namespace ray

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_RAY_RAYGUIBYPASS_HPP__

