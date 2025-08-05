#pragma once

#include <stdlib.h>
#include <termios.h>

// Enums for standard 8-bit colors
typedef enum {
    COLOR_DEFAULT = 0,
    COLOR_BLACK = 30,
    COLOR_RED = 31,
    COLOR_GREEN = 32,
    COLOR_YELLOW = 33,
    COLOR_BLUE = 34,
    COLOR_MAGENTA = 35,
    COLOR_CYAN = 36,
    COLOR_WHITE = 37,
    COLOR_BRIGHT_BLACK = 90,
    COLOR_BRIGHT_RED = 91,
    COLOR_BRIGHT_GREEN = 92,
    COLOR_BRIGHT_YELLOW = 93,
    COLOR_BRIGHT_BLUE = 94,
    COLOR_BRIGHT_MAGENTA = 95,
    COLOR_BRIGHT_CYAN = 96,
    COLOR_BRIGHT_WHITE = 97
} Color;

// Bitmask for text attributes
typedef enum {
    ATTR_NONE = 0,
    ATTR_BOLD = 1 << 0,
    ATTR_UNDERLINE = 1 << 1,
    ATTR_INVERT = 1 << 2
} Attribute;

// The Style struct, our equivalent of a CSS class
typedef struct {
    Color fg;
    Color bg;
    Attribute attr;
} Style;

// Represents a single point on the backstage
typedef struct {
    char character;
    Style style; // Each cell now has a style
} Cell;

// A 2D grid of Cells that represents the desired state of the terminal.
typedef struct {
    int width;
    int height;
    Cell* cells;
} Backstage;

// C-style typedefs for function pointers
typedef void (*PerformanceFunc)(const void* call_sheet, void* inner_monologue, Backstage* backstage, char input_char);
typedef void (*DestructorFunc)(void* call_sheet, void* inner_monologue);

// The Actor struct. It's a bundle of data and function pointers.
// Note: We use 'struct Actor' to allow for self-referencing pointers.
typedef struct Actor {
    PerformanceFunc perform_func;
    DestructorFunc destructor_func;
    const void* call_sheet_data;
    void* inner_monologue_data;

    // New members for the Actor Tree
    struct Actor** children;
    int child_count;
    int child_capacity;
} Actor;

// The Director. Manages the Backstage and orchestrates the Actors.
typedef struct {
    int width;
    int height;
    Backstage* current_backstage;
    Backstage* previous_backstage;
    Actor* root_actor;
} Director;

// The Inner Monologue for our BlinkingTextActor
typedef struct {
    int counter;
    int visible;
} BlinkingState;

// The Inner Monologue for our new LabelActor
typedef struct {
    char* text;
    size_t max_length;
    size_t current_length;
} LabelState;

// The Conductor. The high-level orchestrator of the entire show.
typedef struct {
    Director* director;
    struct termios original_termios;
    int running;
} Conductor;

// All C-callable functions go inside this block.
#ifdef __cplusplus
extern "C" {
#endif
    // Backstage functions
    Backstage* create_backstage(int width, int height);
    void destroy_backstage(Backstage* backstage);
    void backstage_set_character(Backstage* backstage, int x, int y, char c);
    void backstage_set_cell(Backstage* backstage, int x, int y, char c, Style style);
    const Cell* backstage_get_cell(const Backstage* backstage, int x, int y);

    // Style functions
    Style parse_style_string(const char* style_string);
    void apply_style(Style style);
    void reset_style();

    // Director functions
    Director* create_director(int width, int height);
    void destroy_director(Director* director);
    void director_prepare_stage(Director* director);
    void director_present_scene(Director* director);
    void director_cast(Director* director, Actor* actor);
    void director_rehearse(Director* director, char input_char);

    // Conductor functions
    Conductor* create_conductor(int width, int height);
    void destroy_conductor(Conductor* conductor);
    void conductor_run_show(Conductor* conductor);
    void conductor_stop_show(Conductor* conductor);

    // Utility functions
    void enable_raw_mode(Conductor* conductor);
    void disable_raw_mode(Conductor* conductor);
    void clear_screen();
    void move_cursor(int x, int y);

    // Actor definitions and functions
    typedef struct {
        const char* script_line;
        int x;
        int y; // Added missing y coordinate
    } TextCallSheet;

    typedef struct {
        const char* script_line;
        int x;
        int y;
        Style style;
    } StyledTextCallSheet;
    
    // All performance functions must take a generic void* for the inner monologue.
    void text_performance(const TextCallSheet* call_sheet, void* inner_monologue, Backstage* backstage, char input_char);
    void styled_text_performance(const StyledTextCallSheet* call_sheet, void* inner_monologue, Backstage* backstage, char input_char);
    void blink_performance(const TextCallSheet* call_sheet, void* inner_monologue, Backstage* backstage, char input_char);
    void container_performance(const void* call_sheet, void* inner_monologue, Backstage* backstage, char input_char);
    void label_performance(const TextCallSheet* call_sheet, void* inner_monologue, Backstage* backstage, char input_char);

    // C-style wrapper for our casting functions
    Actor* cast_text_actor(const char* script_line, int x, int y);
    Actor* cast_blinking_actor(const char* script_line, int x, int y);
    Actor* cast_container_actor();
    Actor* cast_label_actor(int x, int y, size_t max_length);
    Actor* cast_styled_text_actor(const char* script_line, int x, int y, const char* style_string);
    void add_child(Actor* parent, Actor* child);
    void destroy_actor(Actor* actor);
    void destroy_actor_tree(Actor* actor);

#ifdef __cplusplus
}
#endif
