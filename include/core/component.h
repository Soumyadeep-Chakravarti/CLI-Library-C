// ~/include/core/component.h

#ifndef TUI_COMPONENT_H
#define TUI_COMPONENT_H

#include "render/screen.h"

#include <stddef.h>

typedef struct TUIComponent TUIComponent;

typedef void (*tui_component_render_fn)(TUIComponent *);
typedef void (*tui_component_update_fn)(TUIComponent *);

struct TUIComponent {
    int x, y;
    size_t width, height;
    Cell *state;
    tui_component_render_fn render;
    tui_component_update_fn update;
};

typedef struct{
    int start_x;
    int start_y;
    size_t len;
    void *state;
} Screen;

typedef struct {
    int x, y;
    size_t width, height;
    tui_component_render_fn render;
    tui_component_update_fn update;
    void *state;
// Future additions:
// tui_key_fn on_key;
// tui_focus_fn on_focus;
} TUIComponentProps;

TUIComponent tui_component_create(TUIComponentProps props);

#endif // TUI_COMPONENT_H
