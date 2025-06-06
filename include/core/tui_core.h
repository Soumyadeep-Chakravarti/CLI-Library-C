// ~/include/core/tui_core.h


#ifndef TUI_CORE_H
#define TUI_CORE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*tui_render_fn)(void);

// Initialize TUI core systems (screen, state, etc.)
void tui_core_init(void);

// Run the main render loop (calls `render()` repeatedly)
void tui_core_run(tui_render_fn render);

// Clean up TUI systems
void tui_core_shutdown(void);


// Set the key to trigger exit (default: 'q')
void tui_core_set_exit_key(int key);

#ifdef __cplusplus
}
#endif

#endif // TUI_CORE_H
