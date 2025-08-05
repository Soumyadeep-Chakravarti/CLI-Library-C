#include "termiui.h"
#include <string.h>

void text_performance(const TextCallSheet* call_sheet, void* inner_monologue, Backstage* backstage, char input_char) {
    if (!call_sheet) return;
    for (int i = 0; call_sheet->script_line[i] != '\0'; ++i) {
        backstage_set_character(backstage, call_sheet->x + i, call_sheet->y, call_sheet->script_line[i]);
    }
}

void blink_performance(const TextCallSheet* call_sheet, void* inner_monologue, Backstage* backstage, char input_char) {
    // Cast the generic inner_monologue to our specific state struct
    BlinkingState* state = (BlinkingState*)inner_monologue;

    if (!call_sheet || !state) return;

    state->counter++;
    if (state->counter % 10 == 0) { // Blink every 10 frames
        state->visible = !state->visible;
    }

    if (state->visible) {
        for (int i = 0; call_sheet->script_line[i] != '\0'; ++i) {
            backstage_set_character(backstage, call_sheet->x + i, call_sheet->y, call_sheet->script_line[i]);
        }
    }
}

void container_performance(const void* call_sheet, void* inner_monologue, Backstage* backstage, char input_char) {
    // A container's job is simply to hold other actors. It doesn't draw anything itself.
}

void label_performance(const TextCallSheet* call_sheet, void* inner_monologue, Backstage* backstage, char input_char) {
    LabelState* state = (LabelState*)inner_monologue;
    if (!call_sheet || !state) return;

    if (input_char >= ' ' && input_char <= '~') {
        if (state->current_length < state->max_length) {
            state->text[state->current_length++] = input_char;
            state->text[state->current_length] = '\0';
        }
    } else if (input_char == 127) { // Backspace
        if (state->current_length > 0) {
            state->text[--state->current_length] = '\0';
        }
    }

    for (int i = 0; i < state->current_length; ++i) {
        backstage_set_character(backstage, call_sheet->x + i, call_sheet->y, state->text[i]);
    }
}
