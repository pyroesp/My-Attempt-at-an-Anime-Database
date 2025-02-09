#ifndef _BUTTON_H
#define _BUTTON_H

#include "gui.h"

#define GUI_BUTTON_HEIGHT 40
#define GUI_BUTTON_TEXT_X_OFFSET 14
#define GUI_BUTTON_TEXT_Y_OFFSET 4

#define GUI_BUTTON_BG_COLOR (Color){0x50, 0x50, 0x50, 0xFF}
#define GUI_BUTTON_TEXT_COLOR (Color){0xFF, 0xFF, 0xFF, 0xFF}
#define GUI_BUTTON_HOVER_COLOR (Color){0x80, 0x80, 0x80, 0xFF}
#define GUI_BUTTON_ACTIVE_COLOR (Color){0xF0, 0x80, 0x80, 0xFF}

/*
    Draw a button and return it's value.
    Font size of the label is determined by the height r.h.
    The width of the button is determined by r.w
        If r.w is not 0 then the width is defined by the label and font size.
*/
int gui_button(guiRect r, char* label, int layer, int id);

/* Return the width in pixels of a label based of the button height */
int gui_buttonWidth(char *label, int h);

#endif // _BUTTON_H
