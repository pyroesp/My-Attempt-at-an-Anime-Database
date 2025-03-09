#include <stdio.h>
#include <stdlib.h>

#include "gui/gui.h"
#include "gui/button.h"
#include "gui/dropdown.h"
#include "gui/input_text.h"
#include "gui/scrollback.h"
#include "gui/table.h"

#include "sql/sql.h"
#include "sql/tables/anime.h"
#include "sql/tables/genre.h"
#include "sql/tables/path.h"


int main(int argc, char *argv[]){
    int i, j;
    /// GUI Variables
    int component; // gui component counter ID
    int bar_position = 0; // scrollbar position
    guiRect window = {0, 0, GUI_WINDOW_WIDTH, GUI_WINDOW_HEIGHT};

    int exiting = 0; // exiting state
    int exit_loop = 0; // exit loop variable
    int file_state = 0; // file state
    int view_state = 0; // view state
    int add_state = 0; // add state
    char *add_text = NULL; // add string

    /// Start raylib
    printf("Initializing window...\n");
    gui_init(window.w, window.h, "GUI Test");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);

    /// Setup Menubar
    char *menu[3] = {
        "File",
        "View",
        "Add"
    };
    int menu_x_pos[3] = {0};
    int width = 0;
    for (i = 0; i < 3; i++){
        if (i > 0)
            menu_x_pos[i] = menu_x_pos[i - 1] + width;
        width = gui_buttonWidth(menu[i], GUI_BUTTON_HEIGHT);
        printf("%s = %d\n", menu[i], menu_x_pos[i]);
    }

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

    /// Main loop
    printf("Entering main loop...\n");
    while (!WindowShouldClose() && !exit_loop){
        BeginDrawing();
        ClearBackground(GUI_WINDOW_BG_COLOR);

        // Auto resize window
        gui_windowResize(&window);

        // Reset GUI variables
        component = 0; // reset component id


        gui_openLayer();

        /// Prepare table
        /*
            Prepare table:
            > 5 columns
            > 1 title row

            [   anime   ][ genre ][ episodes ][ watched ][ finished ]        -- TITLE
            [wraparound  [genre   [int        [int       [true/false         -- TABLE DATA
             name       ]        ]           ]          ]           ]
        */
        // table position and size
        guiRect table = {
            .x = 10,
            .y = 10 + GUI_BUTTON_HEIGHT,
            .w = window.w - 10 * 2,
            .h = window.h - 10 * 2 - GUI_BUTTON_HEIGHT
        };
        // create table
        guiTable *t;
        t = gui_initTable(table, anime.count + 1, 5);
        int table_cols_max_width = (table.w - GUI_TABLE_CELL_X_OFFSET*(t->cols+1));
        // col 1 = anime
        gui_setColumnFormat(t, 0, GUI_CELL_WRAPAROUND);
        gui_setColumnWidth(t, 0, 30 * table_cols_max_width / 100); // 30% of the total width
        // col 2 = genre
        gui_setColumnFormat(t, 1, GUI_CELL_WRAPAROUND);
        gui_setColumnWidth(t, 1, 25 * table_cols_max_width / 100); // 20% of the total width
        // col 3 = episodes
        gui_setColumnWidth(t, 2, 15 * table_cols_max_width / 100); // 15% of the total width
        // col 4 = watched
        gui_setColumnWidth(t, 3, 15 * table_cols_max_width / 100); // 15% of the total width
        // col 5 = finished
        gui_setColumnWidth(t, 4, 15 * table_cols_max_width / 100); // 15% of the total width

        /// Set title row
        gui_setRowFormat(t, 0, GUI_CELL_TITLE | GUI_CELL_CENTER);
        gui_setRowHeight(t, 0, GUI_TITLE_DEFAULT_SIZE);
        gui_setRowFontSize(t, 0, GUI_TITLE_DEFAULT_SIZE);
        gui_setCellText(t, 0, 0, "Anime");
        gui_setCellText(t, 0, 1, "Genre");
        gui_setCellText(t, 0, 2, "Episodes");
        gui_setCellText(t, 0, 3, "Watched");
        gui_setCellText(t, 0, 4, "Finished");

        /// Prepare cells
        for (i = 0; i < anime.count; i++){
            gui_setCellText(t, i + 1, 0, ((tableAnime*)anime.table)[i].name);
            gui_setCellText(t, i + 1, 1, ((tableAnime*)anime.table)[i].genre);
            gui_setCellText(t, i + 1, 2, ((tableAnime*)anime.table)[i].episodes);
            gui_setCellText(t, i + 1, 3, ((tableAnime*)anime.table)[i].watched);
            gui_setCellText(t, i + 1, 4, ((tableAnime*)anime.table)[i].finished);
        }

        /// draw table
        gui_table(t);

        // free table
        gui_freeTable(t);

        /*
        guiRect scb = {
            .x = window.w - 2*GUI_SCROLLBAR_WIDTH, // right-side of screen
            .y = GUI_BUTTON_HEIGHT * 2, // below menubar
            .w = 0,
            .h = window.h - GUI_BUTTON_HEIGHT * 3 // height of scrollbar
        };
        int pct = gui_scrollbar(scb, &bar_position, gui_getContext()->active_layer, ++component);
        */

        gui_closeLayer();

        /// Draw Menubar
        // Draw Menubar background
        DrawRectangle(0, 0, window.w, GUI_BUTTON_HEIGHT, GUI_BUTTON_BG_COLOR);
        for (i = 0; i < 3; i++){
            // check button status
            if (gui_button((guiRect){menu_x_pos[i], 0, 0, GUI_BUTTON_HEIGHT}, menu[i], 0, ++component)){
                // if none of the menus are active
                if (!file_state && !view_state && !add_state){
                    // check which state got activated
                    switch (i){
                        case 0: // File
                            file_state = 1;
                            break;
                        case 1: // View
                            view_state = 1;
                            break;
                        case 2: // Add
                            add_state = 1;
                            break;
                        default:
                            break;
                    }
                }
            }
        }

        // if file button pressed, open dropdown and do dropdown logic
        switch (file_state){
            case 1:
                file_state = 2;
                gui_openLayer();
                break;
            case 2:
                int dropdown = gui_dropDown((guiRect){menu_x_pos[0], GUI_BUTTON_HEIGHT, 0, GUI_BUTTON_HEIGHT}, "Load,Save,Quit", 1, ++component);
                switch(dropdown){
                    case 1: // Load
                        // TODO Load or Reload tables into memory
                        // Maybe only if there are no changes to the table to be executed?
                        break;
                    case 2: // Save
                        // TODO: go through sql_list and execute all queries
                        // Maybe reload all data from database afterwards?
                        break;
                    case 3: // Quit
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

        // if view button pressed, open dropdown and do dropdown logic
        switch (view_state){
            case 1:
                view_state = 2;
                gui_openLayer();
                break;
            case 2:
                int dropdown = gui_dropDown((guiRect){menu_x_pos[1], GUI_BUTTON_HEIGHT, 0, GUI_BUTTON_HEIGHT}, "New,Watching,Finished", 1, ++component);
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

        // if add button pressed, open input text and do add logic
        switch (add_state){
            case 1: // Initialize text input on new layer
                add_state = 2;
                gui_openLayer();
                gui_initInputText();
                if (add_text){
                    free(add_text);
                    add_text = NULL;
                }
                break;
            case 2: // Input text
                if (gui_inputText((guiRect){0, 300, window.w, GUI_BUTTON_HEIGHT}, 1, ++component)){
                    add_text = gui_getInputText();
                    add_state = 3;
                }
                break;
            case 3: // Cleanup text input and close layer
                add_state = 0;
                gui_freeInputText();
                gui_closeLayer();
                break;
            case 0:
            default:
                break;
        }

        // If there's text, write it on screen
        if (add_text)
            DrawText(add_text, 0, 400, GUI_BUTTON_HEIGHT, (Color){0xFF, 0, 0, 0xFF});

        // Reset active component if mouse released
        if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            gui_getContext()->active_component = GUI_COMPONENT_RESET;

        EndDrawing();
    }

    if (add_text)
        free(add_text);

    printf("Cleanup sql...\n");
    sql_freeTableAnime((tableAnime*)anime.table, anime.count);
    sql_freeTableGenre((tableGenre*)genre.table, genre.count);
    sql_freeTablePath((tablePath*)path.table, path.count);
    sqlite3_close(db);

    printf("Closing window...\n");
    gui_close();

    printf("Exiting program...\n");
    return 0;
}
