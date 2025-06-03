// ~/include/cli_core.h

#ifndef CLI_CORE_H
#define CLI_CORE_H

int cli_core_init(void);
void cli_core_shutdown(void);
void cli_core_run(void (*render_func)(void));

// control API
void cli_core_request_stop(void);
int cli_core_should_stop(void);

#endif // CLI_CORE_H
