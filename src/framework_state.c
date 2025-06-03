// ~/src/framework_state.c

#include "framework_state.h"

#include <stdlib.h>

FrameworkState *framework_state_init(void) {
    FrameworkState *state = malloc(sizeof(FrameworkState));
    if (!state) return NULL;

    state->frame_count = 0;
    state->window_width = 80;
    state->window_height = 24;
    return state;
}

void framework_state_free(FrameworkState *state) {
    if (state) free(state);
}

void framework_state_update(FrameworkState *state, const FrameworkUpdate *update) {
    if (!state || !update) return;

    state->window_width = update->new_width;
    state->window_height = update->new_height;
    state->frame_count++;
}
