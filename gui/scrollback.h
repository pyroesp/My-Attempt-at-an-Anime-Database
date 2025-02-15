#ifndef _SCROLLBAR_H
#define _SCROLLBAR_H

#include "gui.h"

#define GUI_SCROLLBAR_MAX_VALUE 100
#define GUI_SCROLLBAR_WIDTH 21
#define GUI_SCROLLBAR_HEIGHT 64
#define GUI_SCROLLBAR_Y_OFFSET 20

#define GUI_SCROLLBAR_LINE_COLOR (Color){0x50, 0x50, 0x50, 0xFF}
#define GUI_SCROLLBAR_BAR_COLOR (Color){0x50, 0x50, 0x50, 0xFF}


/*
    GUI scrollbar returns value between 0 and MAX_VALUE.
    bar_y is a pointer to an int keeping the position of the bar.
*/
int gui_scrollbar(guiRect r, int *bar_y, int layer, int id);


#endif // _SCROLLBAR_H
