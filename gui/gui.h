#ifndef _GUI_H
#define _GUI_H

#include <raylib.h>

// Minimum GUI window size
#define GUI_WINDOW_WIDTH 1280
#define GUI_WINDOW_HEIGHT 720

#define GUI_WINDOW_BG_COLOR (Color){0,0,0,0xFF}

// Set a GUI component to always be active (UNTESTED)
#define GUI_LAYER_ALWAYS_ACTIVE -1
#define GUI_COMPONENT_RESET -1

// guiRect structure
typedef struct{
    int x, y;
    int w, h;
}guiRect;

// Gui Context
typedef struct{
    int active_layer;
    int active_component;
}guiContext;

/*
    Return global GUI context variable
*/
guiContext* gui_getContext(void);

/*
    Initialize GUI context and opening raylib window
*/
void gui_init(int w, int h, char *title);
/*
    Close GUI window
*/
void gui_close(void);

/*
    Open new window layer
*/
void gui_openLayer(void);

/*
    Check if layer is active or is GUI_LAYER_ALWAYS_ACTIVE
*/
int gui_isLayerActive(int layer);
/*
    Check if component is active or component value is reset (GUI_COMPONENT_RESET)
*/
int gui_isComponentActive(int component);
/*
    Close window layer
*/
void gui_closeLayer(void);

/*
    Update window size when changed.
    Limit to minimum window size.
*/
void gui_windowResize(guiRect *window);


#endif // _GUI_H
