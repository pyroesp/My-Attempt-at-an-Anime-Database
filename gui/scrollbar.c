#include "scrollback.h"

#include "gui.h"

#include <stdio.h>

int gui_scrollbar(guiRect r, int *bar_y, int layer, int id){
    int percent = 0;
    // set line position
    guiRect line = {
        .x = r.x + GUI_SCROLLBAR_WIDTH/2,
        .y = r.y,
        .w = 0,
        .h = r.h
    };
    // set bar position
    guiRect bar = {
        .x = r.x,
        // set bar position to at least offset/2 pixels below the line
        .y = r.y + GUI_SCROLLBAR_Y_OFFSET/2 + (*bar_y),
        .w = GUI_SCROLLBAR_WIDTH,
        .h = GUI_SCROLLBAR_HEIGHT
    };

    // calc the bar travel based on line height, offset and bar height
    int bar_travel = r.h - GUI_SCROLLBAR_Y_OFFSET - GUI_SCROLLBAR_HEIGHT;

    // if layer active
    if (gui_isLayerActive(layer)){
        // if we are not the active component
        if (gui_getContext()->active_component != id){
            // and if we're inside the bar
            if (gui_isMouseInside(bar)){
                // and the mouse button is down
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
                    // make this the active component
                    gui_getContext()->active_component = id;
                }
            }
        // if we are the active component
        } else {
            // and we're still pressing the mouse button
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
                // set bar_y position to mouse_y position minus all the offsets
                int y_offset = r.y + GUI_SCROLLBAR_Y_OFFSET/2 + GUI_SCROLLBAR_HEIGHT/2;
                (*bar_y) = GetMouseY() - y_offset;
                // check boundaries
                if ((*bar_y) < 0)
                    (*bar_y) = 0;
                else if ((*bar_y) > bar_travel)
                    (*bar_y) = bar_travel;
            }
        }
        // Draw line
        DrawLine(line.x, line.y, line.x, line.y + line.h, GUI_SCROLLBAR_LINE_COLOR);
        // Draw rectangle
        DrawRectangle(bar.x, bar.y, bar.w, bar.h, GUI_SCROLLBAR_BAR_COLOR);
    }
    // calc percent value to return
    percent = ((*bar_y) * 100) / bar_travel;
    return percent;
}

