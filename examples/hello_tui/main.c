// ~/examples/hello_tui/main.c

#include "core/tui_core.h"
#include "render/screen.h"
#include "components/label.h"

#include <string.h>
#include <stdlib.h>

TUIComponent label;

void render(void) {
    screen_clear();
    label.render(&label);  // draw the label
    screen_flush();
}

int main(void) {
    tui_core_init();
    screen_init();

    // Centered "Hello, TUI!" label
    const char *msg = "Hello, TUI!";
    int x = (int)(screen_width() / 2 - strlen(msg) / 2);
    int y = (int)(screen_height() / 2);
    label = tui_label_create(msg, x, y, 15 /* white */, 0 /* black */);

    tui_core_set_exit_key('q');
    tui_core_run(render);

    screen_shutdown();
    tui_core_shutdown();
    return 0;
}
