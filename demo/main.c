#include "cli_core.h"
#include "screen.h"
#include "hot_reload.h"
#include "hotreload_loader.h"

#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>


static int frame = 0;

void render(void) {
    cli_screen_clear();

    int w = cli_screen_width();
    int h = cli_screen_height();

    char msg[] = "Hello from CLI TUI Framework!";
    int msg_len = sizeof(msg) - 1;

    int x = (w - msg_len) / 2;
    int y = h / 2;

    for (int i = 0; i < msg_len; i++) {
        cli_screen_put_char(x + i, y, msg[i]);
    }

    char spinner[] = "-\\|/";
    cli_screen_put_char(w - 2, h - 1, spinner[frame % 4]);
    frame++;
}


reload_hello_t reload_hello = NULL;

int main(void) {

    const char *libhotreload_file_name="libhotreload.so";
    void *libhotreload = dlopen(libhotreload_file_name,RTLD_NOW);
    if (libhotreload == NULL){
        fprintf(stderr,"ERROR: could not load %s: %s",libhotreload_file_name,dlerror());
        return 1;
    }

    reload_hello = (reload_hello_t) dlsym(libhotreload,"reload_hello");
    if (reload_hello == NULL){
        fprintf(stderr,"ERROR: could not find symbol in %s: %s",libhotreload_file_name,dlerror());
        return 1;
    }
    reload_hello();

    return 0;

    if (hotreload_load("./plugins/libhotreload.so") != 0) {
        fprintf(stderr, "Failed to load hotreload module\n");
        cli_core_shutdown();
        return 1;
    }

    // Call the function from the loaded module
    hotreload_call();

    // Unload the hotreload module immediately
    hotreload_unload();

    return 0;


    if (cli_core_init() != 0) {
        fprintf(stderr, "Failed to initialize CLI core\n");
        return 1;
    }

    hotreload_call();

    for (int i = 0; i < 100; i++) {
        cli_core_run(render);
        usleep(100000);
    }

    hotreload_unload();
    cli_core_shutdown();

    return 0;
}
