#include "dropdown.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int gui_dropDown(guiRect r, char *labels, int layer, int id){
    int button = 0;

    char *list;
    list = (char*)malloc(strlen(labels) * sizeof(char));
    strcpy(list, labels);


    int options = gui_getTotalOptions(list);
    int dropdown_height = gui_dropdownHeight(options, r.h);
    int dropdown_width = gui_dropdownWidth(options, list, r.h);

    char *text;

    /// Draw dropdown background
    DrawRectangle(r.x, r.y, dropdown_width, dropdown_height, GUI_BUTTON_BG_COLOR);

    // Get mouse position
    int mouse_x = GetMouseX();
    int mouse_y = GetMouseY();
    int mouse_button = IsMouseButtonDown(MOUSE_BUTTON_LEFT);

    if (!(mouse_x > r.x && mouse_x < (r.x + dropdown_width) && mouse_y > r.y && mouse_y < (r.y + dropdown_height))){
        if (mouse_button && gui_isLayerActive(layer) && gui_getContext()->active_component == GUI_COMPONENT_RESET){
            return -1;
        }
    }

    int i;
    for (i = 0; i < options; i++){
        text = gui_getOptionAtIndex(list, i);

        if (gui_button((guiRect){r.x, r.y + r.h*i, dropdown_width, r.h}, text, layer, id + i))
            button = i + 1;

        free(text);
    }

    free(list);
    list = NULL;
    return button;
}

int gui_getTotalOptions(char *labels){
    int options = 0;
    char *temp = labels;

    // If label length is not 0 and no separator found
    if (strlen(labels) > 0 && strchr(labels, GUI_DROPDOWN_LIST_SEPARATOR) == NULL){
        options = 1;
    } else {
        while(temp){
            options++;
            temp = strchr(temp, GUI_DROPDOWN_LIST_SEPARATOR);
            if (temp){
                if (strlen(temp) <= 1){
                    break;
                }
                temp++;
            }
        }
    }

    return options;
}

char* gui_getOptionAtIndex(char *labels, int index){
    char *label;
    char *start;
    char *separator;
    int length;
    int i = -1;

    start = labels;
    separator = labels;

    do{
        // find separator or null
        separator = strchr(start, GUI_DROPDOWN_LIST_SEPARATOR);
        i++;
        if (i == index){ // if i = index then get the length
            if (separator)
                length = (int)(separator - start) + 1;
            else
                length = strlen(start) + 1;
        } else {
            if (separator)
                if (*separator != 0)
                    start = ++separator;
        }
    }while (i != index);

    label = (char*)calloc(length, sizeof(char));
    strncpy(label, start, length);
    label[length - 1] = 0; // remove separator

    return label;
}

int gui_dropdownHeight(int options, int h){
    return options * h;
}

int gui_dropdownWidth(int options, char *labels, int h){
    int i;
    int width = 0;
    char *label;

    for (i = 0; i < options; i++){
        label = gui_getOptionAtIndex(labels, i);
        if (gui_buttonWidth(label, h) > width)
            width = gui_buttonWidth(label, h);
        free(label);
    }
    return width;
}
