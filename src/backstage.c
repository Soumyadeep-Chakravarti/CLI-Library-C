#include "termiui.h"
#include <stdlib.h>
#include <string.h>

Backstage* create_backstage(int width, int height) {
    Backstage* backstage = (Backstage*)malloc(sizeof(Backstage));
    if (backstage == NULL) return NULL;

    backstage->width = width;
    backstage->height = height;
    backstage->cells = (Cell*)malloc(sizeof(Cell) * width * height);

    if (backstage->cells == NULL) {
        free(backstage);
        return NULL;
    }

    // Initialize all cells to a space character
    for (int i = 0; i < width * height; ++i) {
        backstage->cells[i].character = ' ';
    }
    
    return backstage;
}

void destroy_backstage(Backstage* backstage) {
    if (backstage == NULL) return;
    free(backstage->cells);
    free(backstage);
}

void backstage_set_character(Backstage* backstage, int x, int y, char c) {
    if (x >= 0 && x < backstage->width && y >= 0 && y < backstage->height) {
        backstage->cells[y * backstage->width + x].character = c;
    }
}

const Cell* backstage_get_cell(const Backstage* backstage, int x, int y) {
    if (x >= 0 && x < backstage->width && y >= 0 && y < backstage->height) {
        return &backstage->cells[y * backstage->width + x];
    }
    return NULL;
}
