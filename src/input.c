#include "input.h"
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

static struct termios original_termios;
static char last_key = 0;

int cli_input_init(void) {
    if (tcgetattr(STDIN_FILENO, &original_termios) != 0) return -1;
    return 0;
}

void cli_input_shutdown(void) {
    tcsetattr(STDIN_FILENO, TCSANOW, &original_termios);
}

int cli_input_key_pressed(char key) {
    return last_key == key;
}

void cli_input_poll(void) {
    struct termios oldt, newt;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    newt.c_lflag |= ISIG;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    char ch = 0;
    if (read(STDIN_FILENO, &ch, 1) > 0) {
        last_key = ch;
    } else {
        last_key = 0;
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
}
