#include "raygui.frontend.h"

#define RAYGUI_RICONS_SUPPORT
#define RAYGUI_IMPLEMENTATION
#define RAYGUIDEF static
#include "raygui.h"

void _GuiEnable()
{
    return GuiEnable();
}

void _GuiDisable()
{
    return GuiDisable();
}

void _GuiLock()
{
    return GuiLock();
}

void _GuiUnlock()
{
    return GuiUnlock();
}

void _GuiState(int state)
{
    return GuiState(state);
}

void _GuiFont(Font font)
{
    return GuiFont(font);
}

void _GuiFade(float alpha)
{
    return GuiFade(alpha);
}

void _GuiSetStyle(int control, int property, int value)
{
    return GuiSetStyle(control, property, value);
}

int _GuiGetStyle(int control, int property)
{
    return GuiGetStyle(control, property);
}

bool _GuiWindowBox(Rectangle bounds, char const * text)
{
    return GuiWindowBox(bounds, text);
}

void _GuiGroupBox(Rectangle bounds, char const * text)
{
    return GuiGroupBox(bounds, text);
}

void _GuiLine(Rectangle bounds, char const * text)
{
    return GuiLine(bounds, text);
}

void _GuiPanel(Rectangle bounds)
{
    return GuiPanel(bounds);
}

Rectangle _GuiScrollPanel(Rectangle bounds, Rectangle content, Vector2 * scroll)
{
    return GuiScrollPanel(bounds, content, scroll);
}

void _GuiLabel(Rectangle bounds, char const * text)
{
    return GuiLabel(bounds, text);
}

bool _GuiButton(Rectangle bounds, char const * text)
{
    return GuiButton(bounds, text);
}

bool _GuiLabelButton(Rectangle bounds, char const * text)
{
    return GuiLabelButton(bounds, text);
}

bool _GuiImageButton(Rectangle bounds, Texture2D texture)
{
    return GuiImageButton(bounds, texture);
}

bool _GuiImageButtonEx(Rectangle bounds, Texture2D texture, Rectangle tex_source, char const * text)
{
    return GuiImageButtonEx(bounds, texture, tex_source, text);
}

bool _GuiToggle(Rectangle bounds, char const * text, bool active)
{
    return GuiToggle(bounds, text, active);
}

int _GuiToggleGroup(Rectangle bounds, char const * text, int active)
{
    return GuiToggleGroup(bounds, text, active);
}

bool _GuiCheckBox(Rectangle bounds, char const * text, bool checked)
{
    return GuiCheckBox(bounds, text, checked);
}

int _GuiComboBox(Rectangle bounds, char const * text, int active)
{
    return GuiComboBox(bounds, text, active);
}

bool _GuiDropdownBox(Rectangle bounds, char const * text, int * active, bool edit_mode)
{
    return GuiDropdownBox(bounds, text, active, edit_mode);
}

bool _GuiSpinner(Rectangle bounds, int * value, int min_value, int max_value, bool edit_mode)
{
    return GuiSpinner(bounds, value, min_value, max_value, edit_mode);
}

bool _GuiValueBox(Rectangle bounds, int * value, int min_value, int max_value, bool edit_mode)
{
    return GuiValueBox(bounds, value, min_value, max_value, edit_mode);
}

bool _GuiTextBox(Rectangle bounds, char * text, int text_size, bool edit_mode)
{
    return GuiTextBox(bounds, text, text_size, edit_mode);
}

bool _GuiTextBoxMulti(Rectangle bounds, char * text, int text_size, bool edit_mode)
{
    return GuiTextBoxMulti(bounds, text, text_size, edit_mode);
}

float _GuiSlider(Rectangle bounds, char const * text, float value, float min_value, float max_value, bool show_value)
{
    return GuiSlider(bounds, text, value, min_value, max_value, show_value);
}

float _GuiSliderBar(Rectangle bounds, char const * text, float value, float min_value, float max_value, bool show_value)
{
    return GuiSliderBar(bounds, text, value, min_value, max_value, show_value);
}

float _GuiProgressBar(Rectangle bounds, char const * text, float value, float min_value, float max_value, bool show_value)
{
    return GuiProgressBar(bounds, text, value, min_value, max_value, show_value);
}

void _GuiStatusBar(Rectangle bounds, char const * text)
{
    return GuiStatusBar(bounds, text);
}

void _GuiDummyRec(Rectangle bounds, char const * text)
{
    return GuiDummyRec(bounds, text);
}

int _GuiScrollBar(Rectangle bounds, int value, int min_value, int max_value)
{
    return GuiScrollBar(bounds, value, min_value, max_value);
}

bool _GuiListView(Rectangle bounds, char const * text, int * active, int * scroll_index, bool edit_mode)
{
    return GuiListView(bounds, text, active, scroll_index, edit_mode);
}

bool _GuiListViewEx(Rectangle bounds, char const ** text, int count, int * enabled, int * active, int * focus, int * scroll_index, bool edit_mode)
{
    return GuiListViewEx(bounds, text, count, enabled, active, focus, scroll_index, edit_mode);
}

int _GuiMessageBox(Rectangle bounds, char const * window_title, char const * message, char const * buttons)
{
    return GuiMessageBox(bounds, window_title, message, buttons);
}

Color _GuiColorPicker(Rectangle bounds, Color color)
{
    return GuiColorPicker(bounds, color);
}

Vector2 _GuiGrid(Rectangle bounds, float spacing, int subdivs)
{
    return GuiGrid(bounds, spacing, subdivs);
}

void _GuiLoadStyle(char const * file_name)
{
    return GuiLoadStyle(file_name);
}

void _GuiLoadStyleProps(const int * props, int count)
{
    return GuiLoadStyleProps(props, count);
}

void _GuiLoadStyleDefault()
{
    return GuiLoadStyleDefault();
}

void _GuiUpdateStyleComplete()
{
    return GuiUpdateStyleComplete();
}

