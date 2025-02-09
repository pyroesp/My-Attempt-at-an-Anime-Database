#include "gui.h"


guiContext context;


void gui_init(int w, int h, char *title){
    context.active_layer = 0;
    context.active_component = 0;
    InitWindow(w, h, title);
}

void gui_close(void){
    CloseWindow();
}

guiContext* gui_getContext(void){
    return &context;
}

void gui_openLayer(void){
    ++context.active_layer;
}

int gui_isLayerActive(int layer){
    if (context.active_layer == layer || layer == GUI_LAYER_ALWAYS_ACTIVE)
        return 1;
    return 0;
}

int gui_isComponentActive(int component){
    if (context.active_component == component || context.active_component == GUI_COMPONENT_RESET)
        return 1;
    return 0;
}

void gui_closeLayer(void){
    if (context.active_layer)
        --context.active_layer;
}


void gui_windowResize(guiRect *window){
    int window_resized = 0;
    guiRect new_window;
    // Get window size in case it changed + limit window size
    new_window.w = GetScreenWidth();
    if (window->w != new_window.w){
        window->w = new_window.w < GUI_WINDOW_WIDTH ? GUI_WINDOW_WIDTH : new_window.w;
        window_resized = 1;
    }
    new_window.h = GetScreenHeight();
    if (window->h != new_window.h){
        window->h = new_window.h < GUI_WINDOW_HEIGHT ? GUI_WINDOW_HEIGHT : new_window.h;
        window_resized = 1;
    }
    if (window_resized){
        window_resized = 0;
        SetWindowSize(window->w, window->h);
    }
}
