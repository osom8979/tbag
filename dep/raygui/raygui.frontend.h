#ifndef RAYGUI_FRONTEND_H
#define RAYGUI_FRONTEND_H

#include <raylib.h>

void _GuiEnable();
void _GuiDisable();
void _GuiLock();
void _GuiUnlock();
void _GuiState(int state);
void _GuiFont(Font font);
void _GuiFade(float alpha);

void _GuiSetStyle(int control, int property, int value);
int _GuiGetStyle(int control, int property);

bool _GuiWindowBox(Rectangle bounds, char const * text);
void _GuiGroupBox(Rectangle bounds, char const * text);
void _GuiLine(Rectangle bounds, char const * text);
void _GuiPanel(Rectangle bounds);
Rectangle _GuiScrollPanel(Rectangle bounds, Rectangle content, Vector2 * scroll);

void _GuiLabel(Rectangle bounds, char const * text);
bool _GuiButton(Rectangle bounds, char const * text);
bool _GuiLabelButton(Rectangle bounds, char const * text);
bool _GuiImageButton(Rectangle bounds, Texture2D texture);
bool _GuiImageButtonEx(Rectangle bounds, Texture2D texture, Rectangle tex_source, char const * text);
bool _GuiToggle(Rectangle bounds, char const * text, bool active);
int _GuiToggleGroup(Rectangle bounds, char const * text, int active);
bool _GuiCheckBox(Rectangle bounds, char const * text, bool checked);
int _GuiComboBox(Rectangle bounds, char const * text, int active);
bool _GuiDropdownBox(Rectangle bounds, char const * text, int *active, bool edit_mode);
bool _GuiSpinner(Rectangle bounds, int * value, int min_value, int max_value, bool edit_mode);
bool _GuiValueBox(Rectangle bounds, int * value, int min_value, int max_value, bool edit_mode);
bool _GuiTextBox(Rectangle bounds, char * text, int text_size, bool edit_mode);
bool _GuiTextBoxMulti(Rectangle bounds, char * text, int text_size, bool edit_mode);
float _GuiSlider(Rectangle bounds, char const * text, float value, float min_value, float max_value, bool show_value);
float _GuiSliderBar(Rectangle bounds, char const * text, float value, float min_value, float max_value, bool show_value);
float _GuiProgressBar(Rectangle bounds, char const * text, float value, float min_value, float max_value, bool show_value);
void _GuiStatusBar(Rectangle bounds, char const * text);
void _GuiDummyRec(Rectangle bounds, char const * text);
int _GuiScrollBar(Rectangle bounds, int value, int min_value, int max_value);

bool _GuiListView(Rectangle bounds, char const * text, int * active, int * scroll_index, bool edit_mode);
bool _GuiListViewEx(Rectangle bounds, char const ** text, int count, int * enabled, int * active, int * focus, int * scroll_index, bool edit_mode);
int _GuiMessageBox(Rectangle bounds, char const * window_title, char const * message, char const * buttons);
Color _GuiColorPicker(Rectangle bounds, Color color);
Vector2 _GuiGrid(Rectangle bounds, float spacing, int subdivs);

void _GuiLoadStyle(char const * file_name);
void _GuiLoadStyleProps(const int * props, int count);
void _GuiLoadStyleDefault();
void _GuiUpdateStyleComplete();

#endif // RAYGUI_FRONTEND_H

