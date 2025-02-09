#include "button.h"


int gui_button(guiRect r, char* label, int layer, int id){
    // Get gui context
    guiContext *c = gui_getContext();

    int font_size = r.h - GUI_BUTTON_TEXT_Y_OFFSET*2;
    int label_x = r.x + GUI_BUTTON_TEXT_X_OFFSET;
    int label_y = r.y + GUI_BUTTON_TEXT_Y_OFFSET;
    int button_state = 0;

    // Set button width
    int button_width = r.w ? r.w : gui_buttonWidth(label, r.h);

    // Get mouse position
    int mouse_x = GetMouseX();
    int mouse_y = GetMouseY();
    int mouse_button = IsMouseButtonDown(MOUSE_BUTTON_LEFT);

    /// Draw Button Background
    // If mouse position over button, then draw active/hover rectangle
    if ((mouse_x > r.x && mouse_x < (r.x + button_width)) && (mouse_y > r.y && mouse_y < (r.y + r.h))){
        // Only activate button on mouse press when component and layer are active
        if (mouse_button && gui_isComponentActive(id) && gui_isLayerActive(layer)){
            /// Draw Button Active Background
            DrawRectangle(r.x, r.y, button_width, r.h, GUI_BUTTON_ACTIVE_COLOR);
            c->active_component = id;
            button_state = 1;
        } else {
            /// Draw Button Hover Background
            DrawRectangle(r.x, r.y, button_width, r.h, GUI_BUTTON_HOVER_COLOR);
        }
    }
    /// Draw Button Text
    DrawText(label, label_x, label_y, font_size, GUI_BUTTON_TEXT_COLOR);
    return button_state;
}


int gui_buttonWidth(char *label, int h){
    return MeasureText(label, h - GUI_BUTTON_TEXT_Y_OFFSET*2) + GUI_BUTTON_TEXT_X_OFFSET*2;
}
