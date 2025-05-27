// ~/include/screen.h

#ifndef SCREEN_H
#define SCREEN_H

int cli_screen_init(void);
void cli_screen_shutdown(void);
void cli_screen_clear(void);
void cli_screen_put_char(int x, int y, char c);
void cli_screen_present(void);

int cli_screen_width(void);
int cli_screen_height(void);

#endif // SCREEN_H
