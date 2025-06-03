// ~/include/hot_reload.h

#ifndef HOT_RELOAD_H_

#define HOT_RELOAD_H_

#include "framework_state.h"

typedef struct {
    FrameworkState *framework_state  // internal framework-managed state
    void *user_state;       // user-defined state
}HotReloadContext;

typedef void (*reload_hello_t)(void);

typedef void (*reload_frame_t)(HotReloadContext *ctx);

#endif // HOT_RELOAD_H_
