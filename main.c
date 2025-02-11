#include <stdio.h>
#include <stdlib.h>

#include "gui/gui.h"
#include "gui/button.h"
#include "gui/dropdown.h"
#include "gui/text.h"
#include "gui/scrollback.h"

#include "sql/sql.h"
#include "sql/tables/anime.h"
#include "sql/tables/genre.h"
#include "sql/tables/path.h"


int main(int argc, char *argv[]){
    /// GUI Variables
    int component; // gui component counter ID
    int menubar_x; // menubar x position
    guiRect window = {0, 0, GUI_WINDOW_WIDTH, GUI_WINDOW_HEIGHT};

    int exiting = 0; // exiting state
    int exit_loop = 0; // exit loop variable
    int file_state = 0; // file state
    int view_state = 0; // view state
    int add_state = 0; // add state
    char *add_text = NULL; // add string

    int bar_position = 0; // scrollbar position

    /// SQL stuff
    int sql_list_counter = 0;
    sqlOpList *sql_list = NULL;
    int sql_res;
    char *sql_errmsg = NULL;
    sqlite3 *db;
    sql_res = sqlite3_open(SQL_DATABASE_PATH, &db);
    if (sql_res != SQLITE_OK){
        sql_errmsg = (char*)sqlite3_errmsg(db);
        printf("Cannot open database: %s\n", sql_errmsg);
        sqlite3_free(sql_errmsg);
        sqlite3_close(db);
        return 1;
    }
    printf("Database opened...\n");

    // Test reading anime table
    printf("Reading Anime table...\n");
    sqlTable anime = {0, NULL};
    sql_res = sqlite3_exec(db, TABLE_ANIME_SELECT, sql_animeCallback, (void*)&anime, &sql_errmsg);
    if (sql_res != SQLITE_OK){
        sql_errmsg = (char*)sqlite3_errmsg(db);
        printf("Cannot read table Anime: %s\n", sql_errmsg);
        sqlite3_free(sql_errmsg);
        sqlite3_close(db);
        return 1;
    }
    sql_printAnime((tableAnime*)anime.table, anime.count);

    // Test reading genre table
    printf("Reading Genre table...\n");
    sqlTable genre = {0, NULL};
    sql_res = sqlite3_exec(db, TABLE_GENRE_SELECT, sql_genreCallback, (void*)&genre, &sql_errmsg);
    if (sql_res != SQLITE_OK){
        sql_errmsg = (char*)sqlite3_errmsg(db);
        printf("Cannot read table Genre: %s\n", sql_errmsg);
        sqlite3_free(sql_errmsg);
        sqlite3_close(db);
        return 1;
    }
    sql_printGenre((tableGenre*)genre.table, genre.count);

    // Test reading path table
    printf("Reading Path table...\n");
    sqlTable path = {0, NULL};
    sql_res = sqlite3_exec(db, TABLE_PATH_SELECT, sql_pathCallback, (void*)&path, &sql_errmsg);
    if (sql_res != SQLITE_OK){
        sql_errmsg = (char*)sqlite3_errmsg(db);
        printf("Cannot read table Path: %s\n", sql_errmsg);
        sqlite3_free(sql_errmsg);
        sqlite3_close(db);
        return 1;
    }
    sql_printPath((tablePath*)path.table, path.count);

    /// Start raylib
    printf("Initializing window...\n");
    gui_init(window.w, window.h, "GUI Test");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);

    /// Main loop
    printf("Entering main loop...\n");
    while (!WindowShouldClose() && !exit_loop){
        BeginDrawing();
        ClearBackground(GUI_WINDOW_BG_COLOR);

        // Auto resize window
        gui_windowResize(&window);

        // Reset GUI variables
        menubar_x = 0; // reset x position for menubar buttons
        component = 0; // reset component id


        gui_openLayer();
        /// TODO: textbox for anime table data

        guiRect scb = {
            .x = window.w - 2*GUI_SCROLLBAR_WIDTH, // right-side of screen
            .y = GUI_BUTTON_HEIGHT * 2, // below menubar
            .w = 0,
            .h = window.h - GUI_BUTTON_HEIGHT * 3 // height of scrollbar
        };
        int pct = gui_scrollbar(scb, &bar_position, gui_getContext()->active_layer, ++component);
        printf("scrollbar: %d - %d - %d\n", bar_position, pct, anime.count * pct / 100);

        gui_closeLayer();

        // Draw Menubar background
        DrawRectangle(0, 0, window.w, GUI_BUTTON_HEIGHT, GUI_BUTTON_BG_COLOR);
        // File button
        if (gui_button((guiRect){menubar_x, 0, 0, GUI_BUTTON_HEIGHT}, "File", 0, ++component)){
            if (file_state == 0)
                file_state = 1;
        }
        // if file button pressed, open dropdown and do dropdown logic
        switch (file_state){
            case 1:
                file_state = 2;
                gui_openLayer();
                break;
            case 2:
                int dropdown = gui_dropDown((guiRect){menubar_x, GUI_BUTTON_HEIGHT, 0, GUI_BUTTON_HEIGHT}, "Load,Save,Quit", 1, ++component);
                switch(dropdown){
                    case 1: // option 1
                        // TODO Load or Reload tables into memory
                        // Maybe only if there are no changes to the table to be executed?
                        break;
                    case 2: // option 2
                        // TODO: go through sql_list and execute all queries
                        // Maybe reload all data from database afterwards?
                        break;
                    case 3: // option 3
                        // TODO: Check if there are changes to save before exiting
                        if (sql_list_counter == 0){
                            exiting = 1;
                            file_state = 3;
                        }
                        break;
                    case -1: // click outside dropdown
                        file_state = 3;
                        break;
                    case 0:
                    default:
                        break;
                }
                break;
            case 3: // wait for mouse button to be released
                if (IsMouseButtonUp(MOUSE_BUTTON_LEFT))
                    file_state = 4;
                break;
            case 4: // Cleanup dropdown
                if (exiting)
                    exit_loop = 1;
                file_state = 0;
                gui_closeLayer();
            case 0:
            default:
                break;
        }
        menubar_x += gui_buttonWidth("File", GUI_BUTTON_HEIGHT);

        // View button
        if(gui_button((guiRect){menubar_x, 0, 0, GUI_BUTTON_HEIGHT}, "View", 0, ++component)){
            if (view_state == 0)
                view_state = 1;
        }
        // if view button pressed, open dropdown and do dropdown logic
        switch (view_state){
            case 1:
                view_state = 2;
                gui_openLayer();
                break;
            case 2:
                int dropdown = gui_dropDown((guiRect){menubar_x, GUI_BUTTON_HEIGHT, 0, GUI_BUTTON_HEIGHT}, "New,Watching,Finished", 1, ++component);
                switch(dropdown){
                    case 1: // option 1
                    case 2: // option 2
                    case 3: // option 3
                        break;
                    case -1: // click outside dropdown
                        view_state = 3;
                        break;
                    case 0:
                    default:
                        break;
                }
                break;
            case 3: // wait for mouse button to be released
                if (IsMouseButtonUp(MOUSE_BUTTON_LEFT))
                    view_state = 4;
                break;
            case 4: // Cleanup dropdown
                view_state = 0;
                gui_closeLayer();
            case 0:
            default:
                break;
        }
        menubar_x += gui_buttonWidth("View", GUI_BUTTON_HEIGHT);

        // Add button - test text input
        if(gui_button((guiRect){menubar_x, 0, 0, GUI_BUTTON_HEIGHT}, "Add", 0, ++component)){
            if (add_state == 0)
                add_state = 1;
        }
        // if add button pressed, open input text and do add logic
        switch (add_state){
            case 1: // Initialize text input on new layer
                add_state = 2;
                gui_openLayer();
                gui_initText();
                if (add_text){
                    free(add_text);
                    add_text = NULL;
                }
                break;
            case 2: // Input text
                if (gui_inputText((guiRect){0, 300, window.w, GUI_BUTTON_HEIGHT}, 1, ++component)){
                    add_text = gui_getText();
                    add_state = 3;
                }
                break;
            case 3: // Cleanup text input and close layer
                add_state = 0;
                gui_freeText();
                gui_closeLayer();
                break;
            case 0:
            default:
                break;
        }
        menubar_x += gui_buttonWidth("Add", GUI_BUTTON_HEIGHT);

        // Clear button - clear text input
        if(gui_button((guiRect){menubar_x, 0, 0, GUI_BUTTON_HEIGHT}, "Clear", 0, ++component)){
            if (add_text){
                free(add_text);
                add_text = NULL;
            }
        }
        menubar_x += gui_buttonWidth("Clear", GUI_BUTTON_HEIGHT);

        if (!add_state || !file_state){
            // If there's text, write it on screen
            if (add_text)
                DrawText(add_text, 0, 400, GUI_BUTTON_HEIGHT, (Color){0xFF, 0, 0, 0xFF});
            // Reset active component if mouse released
            if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                gui_getContext()->active_component = GUI_COMPONENT_RESET;
        }

        EndDrawing();
    }

    if (add_text)
        free(add_text);

    printf("Cleanup sql...\n");
    sql_freeTableAnime((tableAnime*)anime.table, anime.count);
    sql_freeTableGenre((tableGenre*)genre.table, genre.count);
    sqlite3_close(db);

    printf("Closing window...\n");
    gui_close();

    printf("Exiting program...\n");
    return 0;
}
