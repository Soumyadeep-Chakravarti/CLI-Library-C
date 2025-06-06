#ifndef TUI_COMPONENT_LABEL_H
#define TUI_COMPONENT_LABEL_H

#include "core/component.h"

TUIComponent tui_label_create(const char *text, int x, int y, uint8_t fg, uint8_t bg);

#endif // TUI_COMPONENT_LABEL_H
