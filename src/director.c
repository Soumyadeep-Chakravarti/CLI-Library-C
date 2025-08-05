#include "termiui.h"
#include <stdio.h>
#include <string.h>

Director* create_director(int width, int height) {
    Director* director = (Director*)malloc(sizeof(Director));
    if (director == NULL) return NULL;

    director->width = width;
    director->height = height;
    director->current_backstage = create_backstage(width, height);
    director->previous_backstage = create_backstage(width, height);
    director->root_actor = NULL;

    if (director->current_backstage == NULL || director->previous_backstage == NULL) {
        destroy_director(director);
        return NULL;
    }
    return director;
}

void destroy_director(Director* director) {
    if (director == NULL) return;
    destroy_backstage(director->current_backstage);
    destroy_backstage(director->previous_backstage);
    if (director->root_actor) {
        destroy_actor_tree(director->root_actor);
    }
    free(director);
}

void director_prepare_stage(Director* director) {
    printf("\033[?25l");
    clear_screen();
    for (int i = 0; i < director->width * director->height; ++i) {
        director->previous_backstage->cells[i].character = ' ';
    }
}

void diff_and_update(Director* director) {
    Style last_style = {COLOR_DEFAULT, COLOR_DEFAULT, ATTR_NONE};
    
    for (int y = 0; y < director->height; ++y) {
        for (int x = 0; x < director->width; ++x) {
            const Cell* current_cell = backstage_get_cell(director->current_backstage, x, y);
            const Cell* previous_cell = backstage_get_cell(director->previous_backstage, x, y);

            // Compare both character and style
            if (current_cell->character != previous_cell->character ||
                current_cell->style.fg != previous_cell->style.fg ||
                current_cell->style.bg != previous_cell->style.bg ||
                current_cell->style.attr != previous_cell->style.attr) {
                
                // If style has changed, apply the new style
                if (current_cell->style.fg != last_style.fg ||
                    current_cell->style.bg != last_style.bg ||
                    current_cell->style.attr != last_style.attr) {
                    
                    move_cursor(x, y);
                    apply_style(current_cell->style);
                    last_style = current_cell->style;
                }

                move_cursor(x, y);
                printf("%c", current_cell->character);
            }
        }
    }
    fflush(stdout);

    // Swap backstages for the next frame
    Backstage* temp = director->previous_backstage;
    director->previous_backstage = director->current_backstage;
    director->current_backstage = temp;
    
    // Reset style at the end of the screen to avoid bleeding
    reset_style();
}

void director_present_scene(Director* director) {
    diff_and_update(director);
}

void director_cast(Director* director, Actor* actor) {
    if (director->root_actor) {
        destroy_actor_tree(director->root_actor);
    }
    director->root_actor = actor;
}

// A helper function for recursive rehearsal
void rehearse_actor_tree(Actor* actor, Backstage* backstage, char input_char) {
    if (!actor) return;
    
    // Perform the actor's own logic
    actor->perform_func(actor->call_sheet_data, actor->inner_monologue_data, backstage, input_char);

    // Recursively rehearse all children
    for (int i = 0; i < actor->child_count; ++i) {
        rehearse_actor_tree(actor->children[i], backstage, input_char);
    }
}

void director_rehearse(Director* director, char input_char) {
    if (director->root_actor) {
        // Clear the current backstage before rehearsal
        Style default_style = {COLOR_DEFAULT, COLOR_DEFAULT, ATTR_NONE};
        for (int i = 0; i < director->width * director->height; ++i) {
            director->current_backstage->cells[i].character = ' ';
            director->current_backstage->cells[i].style = default_style;
        }
        rehearse_actor_tree(director->root_actor, director->current_backstage, input_char);
    }
}
