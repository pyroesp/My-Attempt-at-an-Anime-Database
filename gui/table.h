#ifndef _TABLE_H
#define _TABLE_H

#include "gui.h"


#define GUI_TABLE_CELL_X_OFFSET 6
#define GUI_TABLE_CELL_Y_OFFSET 6
#define GUI_TABLE_BG_COLOR (Color){0x17,0x17,0x17,0xFF}

#define GUI_TITLE_DEFAULT_SIZE 48
#define GUI_TITLE_BG_COLOR DARKBLUE
#define GUI_TITLE_TEXT_COLOR WHITE

#define GUI_CELL_DEFAULT_SIZE 38
#define GUI_CELL_TEXT_X_OFFSET 5
#define GUI_CELL_TEXT_Y_OFFSET 3
#define GUI_CELL_BG_COLOR DARKGRAY
#define GUI_CELL_TEXT_COLOR WHITE


typedef enum{
    GUI_CELL_NORMAL = 0x1,
    GUI_CELL_TITLE = 0x2,
    GUI_CELL_CENTER = 0x4,
    GUI_CELL_WRAPAROUND = 0x8
}guiCellFormat;

typedef struct{
    guiCellFormat format; // format
    char *original_text; // pointer to original text, user has to free this
    int use_display_text; // set to 1 if display_text needs to be used
    char *display_text; // display text if wrap around needed, freed by guiTable

    int font_size; // font size
}guiCell;

typedef struct{
    guiRect r; // table position and full width / height
    int rows, cols; // number of rows and columns
    int *col_width; // column width
    int *row_height; // row height
    guiCell **cell; // cell data
}guiTable;


guiTable* gui_initTable(guiRect r, int rows, int cols);

void gui_table(guiTable *t);

void gui_setCellText(guiTable *t, int row, int col, char *text);

void gui_setCellFormat(guiTable *t, int row, int col, guiCellFormat format);
void gui_setRowFormat(guiTable *t, int row, guiCellFormat format);
void gui_setColumnFormat(guiTable *t, int col, guiCellFormat format);

void gui_setCellFontSize(guiTable *t, int row, int col, int font_size);
void gui_setRowFontSize(guiTable *t, int row, int font_size);
void gui_setColumnFontSize(guiTable *t, int col, int font_size);

void gui_setRowHeight(guiTable *t, int row, int height);
void gui_setColumnWidth(guiTable *t, int col, int width);

void gui_freeTable(guiTable *t);


void gui_wrapAroundCell(guiCell *c, int *row_height, int col_width);



#endif // _TABLE_H
