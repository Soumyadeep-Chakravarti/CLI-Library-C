#include <stdio.h>

#include "hot_reload.h"

void reload_hello(void){
    printf("Hello hot reloaded \n");
    fflush(stdout);
}
