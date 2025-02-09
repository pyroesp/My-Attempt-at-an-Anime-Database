#ifndef _TEXT_H
#define _TEXT_H

#include "gui.h"

#define GUI_TEXT_SIZE_DEFAULT 64
#define GUI_TEXT_X_OFFSET 10
#define GUI_TEXT_Y_OFFSET 2
#define GUI_TEXT_BG_COLOR (Color){0x50, 0x50, 0x50, 0xFF}
#define GUI_TEXT_COLOR (Color){0xFF, 0xFF, 0xFF, 0xFF}

// GUI Text Input struct
typedef struct{
    char *buffer;
    int size;
    int position;
    int finished;
}guiText;

/* Initialize global text input struct */
void gui_initText(void);

/* Free Text Input data when done */
void gui_freeText(void);

/* Return pointer to string when text input is done */
char* gui_getText(void);

/*
    Create a text input that uses the keyboard to fill a string.
    > Press enter to finish.
    > Press backspace to delete chars in string
*/
int gui_inputText(guiRect r, int layer, int id);


#endif // _TEXT_H
