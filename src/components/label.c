#include "core/component.h"
#include "render/screen.h"

#include <string.h>
#include <stdlib.h>

// Label state
typedef struct {
    const char *text;
    uint8_t fg;
    uint8_t bg;
} LabelState;




static void label_render(TUIComponent *self, Screen *screen) {
    LabelState *label = (LabelState *)self->state;
    size_t len = strlen(label->text);
    
    screen->start_x = self->x;
    screen->start_y = self->y;
    screen->state = label;

    for (size_t i = 0; i < len && i < self->width; ++i) {
        screen->start_x += (int)i;
        screen->(Cell)state->text[i];    
        screen_draw(screen);
    }
}

static void label_update(TUIComponent *self) {
    // Optional: dynamic updates (e.g., animations)
    (void)self;
}

TUIComponent tui_label_create(const char *text, int x, int y, uint8_t fg, uint8_t bg) {
    LabelState *state = malloc(sizeof(LabelState));
    state->text = text;
    state->fg = fg;
    state->bg = bg;

    TUIComponentProps props = {
        .x = x,
        .y = y,
        .width = strlen(text),
        .height = 1,
        .render = label_render,
        .update = label_update,
        .state = state
    };

    return tui_component_create(props);
}
