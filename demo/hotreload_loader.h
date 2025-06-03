#ifndef HOTRELOAD_LOADER_H
#define HOTRELOAD_LOADER_H

#include <stddef.h>

typedef void (*reload_func_t)(void);

int hotreload_load(const char *path);
void hotreload_call(void);
void hotreload_unload(void);

#endif // HOTRELOAD_LOADER_H
