#pragma once

#include <stdlib.h>
#include <termios.h>

// Represents a single point on the backstage
typedef struct {
    char character;
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
    const Cell* backstage_get_cell(const Backstage* backstage, int x, int y);

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

    // Input and utility functions
    void enable_raw_mode(Conductor* conductor);
    void disable_raw_mode(Conductor* conductor);
    void clear_screen();
    void move_cursor(int x, int y);

    // Actor definitions and functions
    typedef struct {
        const char* script_line;
        int x;
        int y;
    } TextCallSheet;
    
    // All performance functions must take a generic void* for the inner monologue.
    void text_performance(const TextCallSheet* call_sheet, void* inner_monologue, Backstage* backstage, char input_char);
    void blink_performance(const TextCallSheet* call_sheet, void* inner_monologue, Backstage* backstage, char input_char);
    void container_performance(const void* call_sheet, void* inner_monologue, Backstage* backstage, char input_char);
    void label_performance(const TextCallSheet* call_sheet, void* inner_monologue, Backstage* backstage, char input_char);

    // C-style wrapper for our casting functions
    Actor* cast_text_actor(const char* script_line, int x, int y);
    Actor* cast_blinking_actor(const char* script_line, int x, int y);
    Actor* cast_container_actor();
    Actor* cast_label_actor(int x, int y, size_t max_length);
    void add_child(Actor* parent, Actor* child);
    void destroy_actor(Actor* actor);
    void destroy_actor_tree(Actor* actor);

#ifdef __cplusplus
}
#endif
