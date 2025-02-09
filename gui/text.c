#include "text.h"

#include <stdlib.h>
#include <string.h>


guiText text;


void gui_initText(void){
    text.buffer = NULL;
    text.position = 0;
    text.size = 0;

    text.finished = 0;
}

void gui_freeText(void){
    if (text.buffer){
        free(text.buffer);
        gui_initText();
    }
}

char* gui_getText(void){
    char *s;
    if (text.finished){
        s = (char*)calloc(strlen(text.buffer)+1, sizeof(char));
        strcpy(s, text.buffer);
        return s;
    }
    return NULL;
}

int gui_inputText(guiRect r, int layer, int id){
    if (gui_isLayerActive(layer)){
        if (!text.finished){
            gui_getContext()->active_component = id;
            int font_size = r.h - GUI_TEXT_Y_OFFSET*2;
            if (text.size == 0){
                text.buffer = (char*)calloc(GUI_TEXT_SIZE_DEFAULT, sizeof(char));
                text.size = GUI_TEXT_SIZE_DEFAULT;
            }
            /// Draw Text BgColor
            DrawRectangle(r.x, r.y, r.w, r.h, GUI_TEXT_BG_COLOR);

            // Add '|' as cursor at the end of the string
            if (text.position < text.size){
                text.buffer[text.position] = '|';
            }

            /// Read Keyboard
            int c = GetCharPressed();
            while (c){
                if (text.position < text.size){
                    text.buffer[text.position] = (char)c;
                    text.position++;
                }
                c = GetCharPressed();
            }

            if (IsKeyPressed(KEY_ENTER)){ // if enter pressed
                text.finished = 1;
                // Remove cursor
                if (text.position < text.size){
                    text.buffer[text.position] = 0;
                }
                while(c){ // purge chars pressed if any still in active
                    c = GetCharPressed();
                }
            }else if (IsKeyPressed(KEY_BACKSPACE)){
                if (text.position > 0){
                    // Remove cursor
                    if (text.position < text.size){
                        text.buffer[text.position] = 0;
                    }
                    // Remove last char
                    text.buffer[text.position - 1] = 0;
                    text.position--;
                }
            }

            /// Draw Text
            DrawText(text.buffer, r.x + GUI_TEXT_X_OFFSET, r.y + GUI_TEXT_Y_OFFSET, font_size, GUI_TEXT_COLOR);
        }
    }
    return text.finished;
}
