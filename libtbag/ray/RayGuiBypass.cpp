/**
 * @file   RayGuiBypass.cpp
 * @brief  RayGuiBypass class implementation.
 * @author zer0
 * @date   2019-04-20
 */

// ========================================
// [WARNING] Don't change the order of including.
#include <libtbag/ray/RayBypass.hpp>
#define RAYGUIDEF static
#define RAYGUI_IMPLEMENTATION
#define RICONS_IMPLEMENTATION
using namespace libtbag::ray;
#include <libtbag/ray/raygui/ricons.h.impl>
// Migration guide:
//  - VIM COMMAND: '%s/(Rectangle){/{/g'
//  - VIM COMMAND: '%s/(Vector2){/{/g'
//  - VIM COMMAND: '%s/(Vector3){/{/g'
//  - VIM COMMAND: '%s/(Color){/{/g'
#include <libtbag/ray/raygui/raygui.h.impl>
// ========================================

#include <libtbag/ray/RayGuiBypass.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace ray {

void GuiEnable()
{
    return ::GuiEnable();
}

void GuiDisable()
{
    return ::GuiDisable();
}

void GuiLock()
{
    return ::GuiLock();
}

void GuiUnlock()
{
    return ::GuiUnlock();
}

void GuiState(int state)
{
    return ::GuiState(state);
}

void GuiFont(Font font)
{
    return ::GuiFont(font);
}

void GuiFade(float alpha)
{
    return ::GuiFade(alpha);
}

void GuiSetStyle(int control, int property, int value)
{
    return ::GuiSetStyle(control, property, value);
}

int GuiGetStyle(int control, int property)
{
    return ::GuiGetStyle(control, property);
}

bool GuiWindowBox(Rectangle bounds, char const * text)
{
    return ::GuiWindowBox(bounds, text);
}

void GuiGroupBox(Rectangle bounds, char const * text)
{
    return ::GuiGroupBox(bounds, text);
}

void GuiLine(Rectangle bounds, char const * text)
{
    return ::GuiLine(bounds, text);
}

void GuiPanel(Rectangle bounds)
{
    return ::GuiPanel(bounds);
}

Rectangle GuiScrollPanel(Rectangle bounds, Rectangle content, Vector2 * scroll)
{
    return ::GuiScrollPanel(bounds, content, scroll);
}

void GuiLabel(Rectangle bounds, char const * text)
{
    return ::GuiLabel(bounds, text);
}

bool GuiButton(Rectangle bounds, char const * text)
{
    return ::GuiButton(bounds, text);
}

bool GuiLabelButton(Rectangle bounds, char const * text)
{
    return ::GuiLabelButton(bounds, text);
}

bool GuiImageButton(Rectangle bounds, Texture2D texture)
{
    return ::GuiImageButton(bounds, texture);
}

bool GuiImageButtonEx(Rectangle bounds, Texture2D texture, Rectangle tex_source, char const * text)
{
    return ::GuiImageButtonEx(bounds, texture, tex_source, text);
}

bool GuiToggle(Rectangle bounds, char const * text, bool active)
{
    return ::GuiToggle(bounds, text, active);
}

int GuiToggleGroup(Rectangle bounds, char const * text, int active)
{
    return ::GuiToggleGroup(bounds, text, active);
}

bool GuiCheckBox(Rectangle bounds, char const * text, bool checked)
{
    return ::GuiCheckBox(bounds, text, checked);
}

int GuiComboBox(Rectangle bounds, char const * text, int active)
{
    return ::GuiComboBox(bounds, text, active);
}

bool GuiDropdownBox(Rectangle bounds, char const * text, int * active, bool edit_mode)
{
    return ::GuiDropdownBox(bounds, text, active, edit_mode);
}

bool GuiSpinner(Rectangle bounds, int * value, int min_value, int max_value, bool edit_mode)
{
    return ::GuiSpinner(bounds, value, min_value, max_value, edit_mode);
}

bool GuiValueBox(Rectangle bounds, int * value, int min_value, int max_value, bool edit_mode)
{
    return ::GuiValueBox(bounds, value, min_value, max_value, edit_mode);
}

bool GuiTextBox(Rectangle bounds, char * text, int text_size, bool edit_mode)
{
    return ::GuiTextBox(bounds, text, text_size, edit_mode);
}

bool GuiTextBoxMulti(Rectangle bounds, char * text, int text_size, bool edit_mode)
{
    return ::GuiTextBoxMulti(bounds, text, text_size, edit_mode);
}

float GuiSlider(Rectangle bounds, char const * text, float value,
                float min_value, float max_value, bool show_value)
{
    return ::GuiSlider(bounds, text, value, min_value, max_value, show_value);
}

float GuiSliderBar(Rectangle bounds, char const * text, float value,
                   float min_value, float max_value, bool show_value)
{
    return ::GuiSliderBar(bounds, text, value, min_value, max_value, show_value);
}

float GuiProgressBar(Rectangle bounds, char const * text, float value,
                     float min_value, float max_value, bool show_value)
{
    return ::GuiProgressBar(bounds, text, value, min_value, max_value, show_value);
}

void GuiStatusBar(Rectangle bounds, char const * text)
{
    return ::GuiStatusBar(bounds, text);
}

void GuiDummyRec(Rectangle bounds, char const * text)
{
    return ::GuiDummyRec(bounds, text);
}

int GuiScrollBar(Rectangle bounds, int value, int min_value, int max_value)
{
    return ::GuiScrollBar(bounds, value, min_value, max_value);
}

bool GuiListView(Rectangle bounds, char const * text, int * active, int * scroll_index, bool edit_mode)
{
    return ::GuiListView(bounds, text, active, scroll_index, edit_mode);
}

bool GuiListViewEx(Rectangle bounds, char const ** text, int count, int * enabled, int * active, int * focus,
                   int * scroll_index, bool edit_mode)
{
    return ::GuiListViewEx(bounds, text, count, enabled, active, focus, scroll_index, edit_mode);
}

int GuiMessageBox(Rectangle bounds, char const * window_title, char const * message, char const * buttons)
{
    return ::GuiMessageBox(bounds, window_title, message, buttons);
}

Color GuiColorPicker(Rectangle bounds, Color color)
{
    return ::GuiColorPicker(bounds, color);
}

Vector2 GuiGrid(Rectangle bounds, float spacing, int subdivs)
{
    return ::GuiGrid(bounds, spacing, subdivs);
}

void GuiLoadStyle(char const * file_name)
{
    return ::GuiLoadStyle(file_name);
}

void GuiLoadStyleProps(const int * props, int count)
{
    return ::GuiLoadStyleProps(props, count);
}

void GuiLoadStyleDefault()
{
    return ::GuiLoadStyleDefault();
}

void GuiUpdateStyleComplete()
{
    return ::GuiUpdateStyleComplete();
}

} // namespace ray

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

