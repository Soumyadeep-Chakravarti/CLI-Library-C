// ~/include/cli_core.h

#ifndef CLI_CORE_H
#define CLI_CORE_H

int cli_core_init(void);
void cli_core_shutdown(void);
void cli_core_run(void (*render_func)(void));

#endif // CLI_CORE_H
