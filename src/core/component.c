// ~/src/core/component.c

#include "core/component.h"

TUIComponent tui_component_create(TUIComponentProps props){
    return (TUIComponent){
        .x = props.x,
        .y = props.y,
        .width = props.width,
        .height = props.height,
        .render = props.render,
        .update = props.update,
        .state = props.state
    };
}
