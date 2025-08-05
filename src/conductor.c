#include "termiui.h"
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

Conductor* create_conductor(int width, int height) {
    Conductor* conductor = (Conductor*)malloc(sizeof(Conductor));
    if (conductor == NULL) return NULL;

    conductor->director = create_director(width, height);
    if (conductor->director == NULL) {
        free(conductor);
        return NULL;
    }

    conductor->running = 1;
    return conductor;
}

void destroy_conductor(Conductor* conductor) {
    if (conductor == NULL) return;
    disable_raw_mode(conductor);
    destroy_director(conductor->director);
    free(conductor);
}

void conductor_run_show(Conductor* conductor) {
    enable_raw_mode(conductor);
    director_prepare_stage(conductor->director);

    while (conductor->running) {
        char c = 0;
        read(STDIN_FILENO, &c, 1);
        
        if (c == 'q') {
            conductor->running = 0;
        }

        director_rehearse(conductor->director, c);
        director_present_scene(conductor->director);

        usleep(50000);
    }

    printf("\033[?25h");
    move_cursor(0, conductor->director->height);
}

void conductor_stop_show(Conductor* conductor) {
    conductor->running = 0;
}
