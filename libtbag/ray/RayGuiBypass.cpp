/**
 * @file   RayGuiBypass.cpp
 * @brief  RayGuiBypass class implementation.
 * @author zer0
 * @date   2019-04-20
 */

#include <libtbag/ray/RayGuiBypass.hpp>
#include <libtbag/Type.hpp>
#include <raygui.frontend.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace ray {

void GuiEnable()
{
    return ::_GuiEnable();
}

void GuiDisable()
{
    return ::_GuiDisable();
}

void GuiLock()
{
    return ::_GuiLock();
}

void GuiUnlock()
{
    return ::_GuiUnlock();
}

void GuiState(int state)
{
    return ::_GuiState(state);
}

void GuiFont(Font font)
{
    return ::_GuiFont(font);
}

void GuiFade(float alpha)
{
    return ::_GuiFade(alpha);
}

void GuiSetStyle(int control, int property, int value)
{
    return ::_GuiSetStyle(control, property, value);
}

int GuiGetStyle(int control, int property)
{
    return ::_GuiGetStyle(control, property);
}

bool GuiWindowBox(Rectangle bounds, char const * text)
{
    return ::_GuiWindowBox(bounds, text);
}

void GuiGroupBox(Rectangle bounds, char const * text)
{
    return ::_GuiGroupBox(bounds, text);
}

void GuiLine(Rectangle bounds, char const * text)
{
    return ::_GuiLine(bounds, text);
}

void GuiPanel(Rectangle bounds)
{
    return ::_GuiPanel(bounds);
}

Rectangle GuiScrollPanel(Rectangle bounds, Rectangle content, Vector2 * scroll)
{
    return ::_GuiScrollPanel(bounds, content, scroll);
}

void GuiLabel(Rectangle bounds, char const * text)
{
    return ::_GuiLabel(bounds, text);
}

bool GuiButton(Rectangle bounds, char const * text)
{
    return ::_GuiButton(bounds, text);
}

bool GuiLabelButton(Rectangle bounds, char const * text)
{
    return ::_GuiLabelButton(bounds, text);
}

bool GuiImageButton(Rectangle bounds, Texture2D texture)
{
    return ::_GuiImageButton(bounds, texture);
}

bool GuiImageButtonEx(Rectangle bounds, Texture2D texture, Rectangle tex_source, char const * text)
{
    return ::_GuiImageButtonEx(bounds, texture, tex_source, text);
}

bool GuiToggle(Rectangle bounds, char const * text, bool active)
{
    return ::_GuiToggle(bounds, text, active);
}

int GuiToggleGroup(Rectangle bounds, char const * text, int active)
{
    return ::_GuiToggleGroup(bounds, text, active);
}

bool GuiCheckBox(Rectangle bounds, char const * text, bool checked)
{
    return ::_GuiCheckBox(bounds, text, checked);
}

int GuiComboBox(Rectangle bounds, char const * text, int active)
{
    return ::_GuiComboBox(bounds, text, active);
}

bool GuiDropdownBox(Rectangle bounds, char const * text, int * active, bool edit_mode)
{
    return ::_GuiDropdownBox(bounds, text, active, edit_mode);
}

bool GuiSpinner(Rectangle bounds, int * value, int min_value, int max_value, bool edit_mode)
{
    return ::_GuiSpinner(bounds, value, min_value, max_value, edit_mode);
}

bool GuiValueBox(Rectangle bounds, int * value, int min_value, int max_value, bool edit_mode)
{
    return ::_GuiValueBox(bounds, value, min_value, max_value, edit_mode);
}

bool GuiTextBox(Rectangle bounds, char * text, int text_size, bool edit_mode)
{
    return ::_GuiTextBox(bounds, text, text_size, edit_mode);
}

bool GuiTextBoxMulti(Rectangle bounds, char * text, int text_size, bool edit_mode)
{
    return ::_GuiTextBoxMulti(bounds, text, text_size, edit_mode);
}

float GuiSlider(Rectangle bounds, char const * text, float value,
                float min_value, float max_value, bool show_value)
{
    return ::_GuiSlider(bounds, text, value, min_value, max_value, show_value);
}

float GuiSliderBar(Rectangle bounds, char const * text, float value,
                   float min_value, float max_value, bool show_value)
{
    return ::_GuiSliderBar(bounds, text, value, min_value, max_value, show_value);
}

float GuiProgressBar(Rectangle bounds, char const * text, float value,
                     float min_value, float max_value, bool show_value)
{
    return ::_GuiProgressBar(bounds, text, value, min_value, max_value, show_value);
}

void GuiStatusBar(Rectangle bounds, char const * text)
{
    return ::_GuiStatusBar(bounds, text);
}

void GuiDummyRec(Rectangle bounds, char const * text)
{
    return ::_GuiDummyRec(bounds, text);
}

int GuiScrollBar(Rectangle bounds, int value, int min_value, int max_value)
{
    return ::_GuiScrollBar(bounds, value, min_value, max_value);
}

bool GuiListView(Rectangle bounds, char const * text, int * active, int * scroll_index, bool edit_mode)
{
    return ::_GuiListView(bounds, text, active, scroll_index, edit_mode);
}

bool GuiListViewEx(Rectangle bounds, char const ** text, int count, int * enabled, int * active, int * focus,
                   int * scroll_index, bool edit_mode)
{
    return ::_GuiListViewEx(bounds, text, count, enabled, active, focus, scroll_index, edit_mode);
}

int GuiMessageBox(Rectangle bounds, char const * window_title, char const * message, char const * buttons)
{
    return ::_GuiMessageBox(bounds, window_title, message, buttons);
}

Color GuiColorPicker(Rectangle bounds, Color color)
{
    return ::_GuiColorPicker(bounds, color);
}

Vector2 GuiGrid(Rectangle bounds, float spacing, int subdivs)
{
    return ::_GuiGrid(bounds, spacing, subdivs);
}

void GuiLoadStyle(char const * file_name)
{
    return ::_GuiLoadStyle(file_name);
}

void GuiLoadStyleProps(const int * props, int count)
{
    return ::_GuiLoadStyleProps(props, count);
}

void GuiLoadStyleDefault()
{
    return ::_GuiLoadStyleDefault();
}

void GuiUpdateStyleComplete()
{
    return ::_GuiUpdateStyleComplete();
}

} // namespace ray

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

