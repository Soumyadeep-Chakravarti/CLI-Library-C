#include "termiui.h"
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

void enable_raw_mode(Conductor* conductor) {
    if (tcgetattr(STDIN_FILENO, &conductor->original_termios) == -1) {
        perror("tcgetattr");
        exit(1);
    }

    struct termios raw = conductor->original_termios;
    raw.c_lflag &= ~(ECHO | ICANON);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;
    
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
        perror("tcsetattr");
        exit(1);
    }
}

void disable_raw_mode(Conductor* conductor) {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &conductor->original_termios) == -1) {
        perror("tcsetattr");
        exit(1);
    }
}

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
