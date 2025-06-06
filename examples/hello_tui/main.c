// ~/examples/hello_tui/main.c

#include "core/tui_core.h"
#include <stdio.h>

void render_hello(void) {
    printf("Rendering hello frame!\n");
}

int main(void) {
    tui_core_init();
    tui_core_run(render_hello);
    tui_core_shutdown();
    return 0;
}
