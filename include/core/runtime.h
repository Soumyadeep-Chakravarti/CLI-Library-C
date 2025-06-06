// ~/include/core/runtime.h

#ifndef CORE_RUNTIME_H
#define CORE_RUNTIME_H

#include <stdbool.h>

typedef void (*RenderCallback)(void);

void tui_core_init(void);
void tui_core_shutdown(void);
void tui_core_run(RenderCallback render);
void tui_core_request_exit(void);
bool tui_core_should_exit(void);

#endif // CORE_RUNTIME_H
