// ~/include/input.h

#ifndef CLI_INPUT_H
#define CLI_INPUT_H

int cli_input_init(void);
void cli_input_shutdown(void);
void cli_input_poll(void);

int cli_input_key_pressed(char key);

#endif
