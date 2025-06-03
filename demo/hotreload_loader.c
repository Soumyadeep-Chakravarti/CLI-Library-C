#include "hotreload_loader.h"

#include <stdio.h>
#include <dlfcn.h>

static void *handle = NULL;
static reload_func_t reload_hello = NULL;

int hotreload_load(const char *path) {
    handle = dlopen(path, RTLD_NOW);
    if (!handle) {
        fprintf(stderr, "dlopen failed: %s\n", dlerror());
        return -1;
    }
    reload_hello = (reload_func_t)dlsym(handle, "reload_hello");
    if (!reload_hello) {
        fprintf(stderr, "dlsym failed: %s\n", dlerror());
        dlclose(handle);
        handle = NULL;
        return -1;
    }
    return 0;
}

void hotreload_call(void) {
    if (reload_hello) {
        reload_hello();
    }
}

void hotreload_unload(void) {
    if (handle) {
        dlclose(handle);
        handle = NULL;
        reload_hello = NULL;
    }
}
