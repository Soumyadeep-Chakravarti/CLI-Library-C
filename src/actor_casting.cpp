#include "termiui.h"
#include <memory>
#include <cstring>
#include <string>

// A C-style destructor for the text actor
void text_call_sheet_destructor(void* call_sheet_data, void* inner_monologue_data) {
    TextCallSheet* cs = static_cast<TextCallSheet*>(call_sheet_data);
    free((void*)cs->script_line); // Free the duplicated string
    delete cs;
}

// C-style destructor for the blinking actor
void blinking_actor_destructor(void* call_sheet_data, void* inner_monologue_data) {
    text_call_sheet_destructor(call_sheet_data, nullptr);
    BlinkingState* state = static_cast<BlinkingState*>(inner_monologue_data);
    delete state;
}

// C-style destructor for the label actor
void label_actor_destructor(void* call_sheet_data, void* inner_monologue_data) {
    TextCallSheet* cs = static_cast<TextCallSheet*>(call_sheet_data);
    delete cs;
    LabelState* state = static_cast<LabelState*>(inner_monologue_data);
    free(state->text);
    delete state;
}

// The destructor for a complete actor tree
void destroy_actor_tree(Actor* actor) {
    if (!actor) return;
    for (int i = 0; i < actor->child_count; ++i) {
        destroy_actor_tree(actor->children[i]);
    }
    if (actor->children) {
        free(actor->children);
    }
    if (actor->destructor_func) {
        actor->destructor_func((void*)actor->call_sheet_data, actor->inner_monologue_data);
    }
    delete actor;
}

// C-style wrapper for casting a text actor
Actor* cast_text_actor(const char* script_line, int x, int y) {
    TextCallSheet* call_sheet = new TextCallSheet;
    call_sheet->script_line = strdup(script_line);
    call_sheet->x = x;
    call_sheet->y = y;

    Actor* actor = new Actor;
    actor->call_sheet_data = static_cast<const void*>(call_sheet);
    actor->inner_monologue_data = nullptr;
    actor->perform_func = reinterpret_cast<PerformanceFunc>(text_performance);
    actor->destructor_func = text_call_sheet_destructor;
    actor->children = nullptr;
    actor->child_count = 0;
    actor->child_capacity = 0;

    return actor;
}

// C-style wrapper for casting a blinking actor
Actor* cast_blinking_actor(const char* script_line, int x, int y) {
    TextCallSheet* call_sheet = new TextCallSheet;
    call_sheet->script_line = strdup(script_line);
    call_sheet->x = x;
    call_sheet->y = y;

    BlinkingState* state = new BlinkingState{0, 1};
    
    Actor* actor = new Actor;
    actor->call_sheet_data = static_cast<const void*>(call_sheet);
    actor->inner_monologue_data = static_cast<void*>(state);
    actor->perform_func = reinterpret_cast<PerformanceFunc>(blink_performance);
    actor->destructor_func = blinking_actor_destructor;
    actor->children = nullptr;
    actor->child_count = 0;
    actor->child_capacity = 0;

    return actor;
}

// C-style wrapper for casting a container actor
Actor* cast_container_actor() {
    Actor* actor = new Actor;
    actor->call_sheet_data = nullptr;
    actor->inner_monologue_data = nullptr;
    actor->perform_func = reinterpret_cast<PerformanceFunc>(container_performance);
    actor->destructor_func = nullptr;
    actor->children = (Actor**)malloc(sizeof(Actor*) * 2);
    actor->child_count = 0;
    actor->child_capacity = 2;
    return actor;
}

// C-style wrapper for casting a label actor
Actor* cast_label_actor(int x, int y, size_t max_length) {
    TextCallSheet* call_sheet = new TextCallSheet;
    call_sheet->script_line = nullptr;
    call_sheet->x = x;
    call_sheet->y = y;

    LabelState* state = new LabelState;
    state->text = (char*)malloc(max_length + 1);
    state->text[0] = '\0';
    state->max_length = max_length;
    state->current_length = 0;
    
    Actor* actor = new Actor;
    actor->call_sheet_data = static_cast<const void*>(call_sheet);
    actor->inner_monologue_data = static_cast<void*>(state);
    actor->perform_func = reinterpret_cast<PerformanceFunc>(label_performance);
    actor->destructor_func = label_actor_destructor;
    actor->children = nullptr;
    actor->child_count = 0;
    actor->child_capacity = 0;

    return actor;
}

// C-style function to add a child to a parent actor
void add_child(Actor* parent, Actor* child) {
    if (!parent || !child) return;
    if (parent->child_count == parent->child_capacity) {
        parent->child_capacity *= 2;
        parent->children = (Actor**)realloc(parent->children, sizeof(Actor*) * parent->child_capacity);
    }
    parent->children[parent->child_count++] = child;
}

void destroy_actor(Actor* actor) {
    // This function now calls the tree destructor, as it's the more
    // appropriate way to clean up any actor.
    destroy_actor_tree(actor);
}
