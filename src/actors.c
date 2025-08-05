#include "termiui.h"
#include <string.h>
#include <stdio.h>

// Helper functions for parsing and applying styles
void apply_style(Style style) {
    printf("\033[0m"); // Reset all attributes first
    if (style.fg != COLOR_DEFAULT) {
        printf("\033[%dm", style.fg);
    }
    if (style.bg != COLOR_DEFAULT) {
        printf("\033[%dm", style.bg + 10);
    }
    if (style.attr & ATTR_BOLD) {
        printf("\033[1m");
    }
    if (style.attr & ATTR_UNDERLINE) {
        printf("\033[4m");
    }
    if (style.attr & ATTR_INVERT) {
        printf("\033[7m");
    }
}

void reset_style() {
    printf("\033[0m");
}

Style parse_style_string(const char* style_string) {
    Style style = {COLOR_DEFAULT, COLOR_DEFAULT, ATTR_NONE};
    if (!style_string) return style;

    char* token_str = strdup(style_string);
    char* token = strtok(token_str, " ");

    while (token) {
        if (strcmp(token, "bold") == 0) style.attr |= ATTR_BOLD;
        else if (strcmp(token, "underline") == 0) style.attr |= ATTR_UNDERLINE;
        else if (strcmp(token, "invert") == 0) style.attr |= ATTR_INVERT;
        else if (strncmp(token, "fg-", 3) == 0) {
            const char* color_name = token + 3;
            if (strcmp(color_name, "red") == 0) style.fg = COLOR_RED;
            else if (strcmp(color_name, "green") == 0) style.fg = COLOR_GREEN;
            else if (strcmp(color_name, "blue") == 0) style.fg = COLOR_BLUE;
            else if (strcmp(color_name, "white") == 0) style.fg = COLOR_WHITE;
        }
        else if (strncmp(token, "bg-", 3) == 0) {
            const char* color_name = token + 3;
            if (strcmp(color_name, "red") == 0) style.bg = COLOR_RED;
            else if (strcmp(color_name, "green") == 0) style.bg = COLOR_GREEN;
            else if (strcmp(color_name, "blue") == 0) style.bg = COLOR_BLUE;
            else if (strcmp(color_name, "white") == 0) style.bg = COLOR_WHITE;
        }

        token = strtok(NULL, " ");
    }

    free(token_str);
    return style;
}

void text_performance(const TextCallSheet* call_sheet, void* inner_monologue, Backstage* backstage, char input_char) {
    if (!call_sheet) return;
    Style default_style = {COLOR_DEFAULT, COLOR_DEFAULT, ATTR_NONE};
    for (int i = 0; call_sheet->script_line[i] != '\0'; ++i) {
        backstage_set_cell(backstage, call_sheet->x + i, call_sheet->y, call_sheet->script_line[i], default_style);
    }
}

void styled_text_performance(const StyledTextCallSheet* call_sheet, void* inner_monologue, Backstage* backstage, char input_char) {
    if (!call_sheet) return;
    for (int i = 0; call_sheet->script_line[i] != '\0'; ++i) {
        backstage_set_cell(backstage, call_sheet->x + i, call_sheet->y, call_sheet->script_line[i], call_sheet->style);
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

    Style default_style = {COLOR_DEFAULT, COLOR_DEFAULT, ATTR_NONE};
    if (state->visible) {
        for (int i = 0; call_sheet->script_line[i] != '\0'; ++i) {
            backstage_set_cell(backstage, call_sheet->x + i, call_sheet->y, call_sheet->script_line[i], default_style);
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

    Style default_style = {COLOR_DEFAULT, COLOR_DEFAULT, ATTR_NONE};
    for (int i = 0; i < state->current_length; ++i) {
        backstage_set_cell(backstage, call_sheet->x + i, call_sheet->y, state->text[i], default_style);
    }
}

