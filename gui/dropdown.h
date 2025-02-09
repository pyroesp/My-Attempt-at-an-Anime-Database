#ifndef _DROPDOWN_H
#define _DROPDOWN_H

/*
    Dropdown is a series of buttons
*/
#include "button.h"


#define GUI_DROPDOWN_LIST_SEPARATOR ','

/*
    Draw dropdown button list and return the pressed button.
    The position of the first dropdown button is defined in r.x and r.y.
    The width and height of each button is defined in r.h and r.w.
    Labels is a string are separated by a comma.
*/
int gui_dropDown(guiRect r, char *labels, int layer, int id);

/* Return a count of labels in the string */
int gui_getTotalOptions(char *labels);

/* Return the label at the given index */
char* gui_getOptionAtIndex(char *labels, int index);

/* Return the height in pixels for the amount of labels in the dropdown */
int gui_dropdownHeight(int options, int h);

/* Return the max width for the given labels in the dropdown */
int gui_dropdownWidth(int options, char *labels, int h);

#endif // _DROPDOWN_H
