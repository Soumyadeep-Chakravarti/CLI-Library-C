#include <stdio.h>
#include <unistd.h>
#include "termiui.h"

int main() {
    Conductor* conductor = create_conductor(80, 24);
    if (!conductor) {
        fprintf(stderr, "Failed to create conductor.\n");
        return 1;
    }
    
    // Create a container actor as the root
    Actor* root_container = cast_container_actor();
    director_cast(conductor->director, root_container);

    // Cast and add actors to the container
    Actor* blinking_actor = cast_blinking_actor("Type here:", 1, 1);
    add_child(root_container, blinking_actor);

    Actor* label_actor = cast_label_actor(12, 1, 60);
    add_child(root_container, label_actor);

    Actor* info_actor = cast_text_actor("Press 'q' to quit, backspace to delete.", 1, 3);
    add_child(root_container, info_actor);

    conductor_run_show(conductor);
    
    destroy_conductor(conductor);

    return 0;
}
