// ~/include/render/screen.h

#ifndef RENDER_SCREEN_H
#define RENDER_SCREEN_H

#include "core/component.h"

#include <stdint.h>
#include <stddef.h>

typedef struct {
    char ch;
    uint8_t fg;
    uint8_t bg;
} Cell;

void screen_init(void);
void screen_shutdown(void);
void screen_clear(void);
void screen_draw(Screen *self);
void screen_flush(void);
size_t screen_width(void);
size_t screen_height(void);

#endif // RENDER_SCREEN_H
