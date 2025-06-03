// ~/include/framework_state.h

#ifndef FRAMEWORK_STATE_H_
#define FRAMEWORK_STATE_H_

typedef struct {

    // Example framework internal states
    int frame_count;
    int window_width;
    int window_height;

    // Add more framework-managed fields here as needed
} FrameworkState;

typedef struct {

    int new_width;
    int new_height;

    // Add more fields as needed, e.g., input events, flags, etc.
} FrameworkUpdate;


// Initializes and returns a pointer to a new FrameworkState
FrameworkState *framework_state_init(void);


// Frees the FrameworkState
void framework_state_free(FrameworkState *state);

void framework_state_update(FrameworkState *state, const FrameworkUpdate *update);

#endif // FRAMEWORK_STATE_H_
