#include <stdio.h>

#define CLAY_IMPLEMENTATION
#include "ui/clay.h"
#include "ui/clay_renderer_raylib.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720


void HandleClayErrors(Clay_ErrorData errorData){
    printf("%s", errorData.errorText.chars);
}


int main(int argc, char *argv[]){
    Clay_Raylib_Initialize(WINDOW_WIDTH, WINDOW_HEIGHT, "MAAAAD", FLAG_WINDOW_RESIZABLE);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(DARKGRAY);

        EndDrawing();
    }

    return 0;
}
