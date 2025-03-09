#include "table.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

guiTable* gui_initTable(guiRect r, int rows, int cols){
    int i, j;
    guiTable *t;
    t = (guiTable*)malloc(sizeof(guiTable));
    t->r = r;
    t->rows = rows;
    t->cols = cols;

    t->col_width = (int*)malloc(cols * sizeof(int));
    for (i = 0; i < cols; i++)
        t->col_width[i] = GUI_CELL_DEFAULT_SIZE;
    t->row_height = (int*)malloc(rows * sizeof(int));
    for (i = 0; i < rows; i++)
        t->row_height[i] = GUI_CELL_DEFAULT_SIZE;

    t->cell = (guiCell**)malloc(rows * sizeof(guiCell*));
    for (i = 0; i < rows; i++){
        t->cell[i] = (guiCell*)malloc(cols * sizeof(guiCell));
        for (j = 0; j < cols; j++){
            t->cell[i][j].original_text = NULL;
            t->cell[i][j].display_text = NULL;
            t->cell[i][j].use_display_text = 0;
            t->cell[i][j].format = GUI_CELL_NORMAL;
            t->cell[i][j].font_size = GUI_CELL_DEFAULT_SIZE;
        }
    }

    return t;
}

void gui_table(guiTable *t){
    int i, j;
    // Draw table background
    DrawRectangle(t->r.x, t->r.y, t->r.w, t->r.h, GUI_TABLE_BG_COLOR);

    // Draw cell background
    int table_row_max_height = (t->r.h - GUI_TABLE_CELL_Y_OFFSET*2);
    int cell_y = GUI_TABLE_CELL_Y_OFFSET;
    for (i = 0; i < t->rows && (cell_y + GUI_TABLE_CELL_Y_OFFSET) < table_row_max_height; i++){
        int cell_x = GUI_TABLE_CELL_X_OFFSET;
        for (j = 0; j < t->cols; j++){
            DrawRectangle(
                t->r.x + cell_x, t->r.y + cell_y, t->col_width[j],
                cell_y + t->row_height[i] < table_row_max_height ? t->row_height[i] : table_row_max_height - cell_y + GUI_TABLE_CELL_Y_OFFSET,
                t->cell[i][j].format & GUI_CELL_TITLE ? GUI_TITLE_BG_COLOR : GUI_CELL_BG_COLOR
            );

            int font_size = t->cell[i][j].font_size - GUI_CELL_TEXT_Y_OFFSET*2;
            char *text = t->cell[i][j].use_display_text ? t->cell[i][j].display_text : t->cell[i][j].original_text;
            DrawText(
                text,
                t->r.x + cell_x + GUI_CELL_TEXT_X_OFFSET + (t->cell[i][j].format & GUI_CELL_CENTER ? (t->col_width[j] - GUI_CELL_TEXT_X_OFFSET*2 - MeasureText(text, font_size))/2 : 0),
                t->r.y + cell_y + GUI_CELL_TEXT_Y_OFFSET,
                font_size,
                GUI_CELL_TEXT_COLOR
            );

            cell_x += t->col_width[j] + GUI_TABLE_CELL_X_OFFSET;
        }
        cell_y += t->row_height[i] + GUI_TABLE_CELL_Y_OFFSET;
    }
}

void gui_setCellText(guiTable *t, int row, int col, char *text){
    if (t && text){
        if (row >= 0 && row < t->rows && col >= 0 && col < t->cols){
            t->cell[row][col].original_text = text;
            gui_wrapAroundCell(&t->cell[row][col], &t->row_height[row], t->col_width[col]);
        }
    }
}

void gui_wrapAroundCell(guiCell *c, int *row_height, int col_width){
    // check if format is wrap around
    if (c->format & GUI_CELL_WRAPAROUND){
        col_width -= GUI_CELL_TEXT_X_OFFSET*2; // calc actual column width
        int font_size = c->font_size - GUI_CELL_TEXT_Y_OFFSET*2; // calc actual text font_size

        // check if original text width is bigger than column width
        if (MeasureText(c->original_text, font_size) > col_width){
            int full_len = strlen(c->original_text); // get original text length
            int space_pos, comma_pos; // help vars for finding spaces and commas
            int text_pos = 0; // initial text position
            int line_end = 1; // line end position
            int lines = 0; // number of lines
            char *line = (char*)calloc(full_len, sizeof(char)); // line string
            char *wrapped = (char*)calloc(full_len*2, sizeof(char)); // full wrapped string

            // loop through the original string
            while (text_pos < full_len){
                // copy line_end size of chars from original text to line
                strncpy(line, &c->original_text[text_pos], line_end);
                // terminate string
                line[line_end] = '\0';

                // check if line width && check if we're not exceeding the original string length
                if (MeasureText(line, font_size) < col_width && text_pos + line_end < full_len){
                    // if width is smaller, or not exceeded full_length then increase line_end
                    line_end++;
                } else {
                    // reset space and comma position
                    space_pos = -1;
                    comma_pos = -1;
                    // loop through line - 1 to find the last comma and space
                    int i;
                    for (i = 0; i < strlen(line) - 1; i++){
                        if (line[i] == ' ')
                            space_pos = i;
                        if (line[i] == ',')
                            comma_pos = i;
                    }

                    // if full length exceeded
                    if (text_pos + line_end >= full_len){
                        strcat(wrapped, line); // add line string to wrapped
                        text_pos += line_end; // increase text_pos with size of line_end
                    }
                    // if no space or comma found
                    else if (space_pos == -1 && comma_pos == -1){
                        // remove 1 from line end to have a string smaller than col_width
                        line_end--;
                        line[line_end] = '\n'; // add new line
                        strcat(wrapped, line); // add line string to wrapped
                        text_pos += line_end; // increase text_pos with size of line_end
                    } else {
                        // if space or comma found, get the furthest position to have a longer string
                        if (space_pos > comma_pos){
                            line[space_pos] = '\n'; // replace space by new line
                            line[space_pos+1] = '\0'; // add string terminator
                            strcat(wrapped, line); // add line string to wrapped
                            text_pos += space_pos + 1; // increase text_pos with size of space_pos
                        } else {
                            line[comma_pos+1] = '\n'; // add new line after comma
                            line[comma_pos+2] = '\0'; // add string terminator after new line
                            strcat(wrapped, line); // add line string to wrapped
                            text_pos += comma_pos + 1; // increase text_pos with size of comma_pos
                        }
                    }

                    lines++; // increase line counter
                    line_end = 1; // reset line_end to 1
                    memset(line, 0, full_len*sizeof(char)); // clear line string
                }
            }
            (*row_height) = lines * c->font_size; // update row_height
            c->display_text = strdup(wrapped); // duplicate wrapped to display_text
            c->use_display_text = 1; // set use_display_text

            // cleanup
            free(wrapped);
            free(line);
        }
    }
}


void gui_setCellFormat(guiTable *t, int row, int col, guiCellFormat format){
    t->cell[row][col].format = format;
}

void gui_setRowFormat(guiTable *t, int row, guiCellFormat format){
    int i;
    for (i = 0; i < t->cols; i++)
        gui_setCellFormat(t, row, i, format);
}

void gui_setColumnFormat(guiTable *t, int col, guiCellFormat format){
    int i;
    for (i = 0; i < t->rows; i++)
        gui_setCellFormat(t, i, col, format);
}


void gui_setCellFontSize(guiTable *t, int row, int col, int font_size){
    t->cell[row][col].font_size = font_size;
}

void gui_setRowFontSize(guiTable *t, int row, int font_size){
    int i;
    for (i = 0; i < t->cols; i++)
        gui_setCellFontSize(t, row, i, font_size);
}

void gui_setColumnFontSize(guiTable *t, int col, int font_size){
    int i;
    for (i = 0; i < t->rows; i++)
        gui_setCellFontSize(t, i, col, font_size);
}

void gui_setRowHeight(guiTable *t, int row, int height){
    if (row <= t->rows)
        t->row_height[row] = height;
}

void gui_setColumnWidth(guiTable *t, int col, int width){
    if (col <= t->cols)
        t->col_width[col] = width;
}

void gui_freeTable(guiTable *t){
    int i,j;
    free(t->col_width);
    free(t->row_height);
    for (i = 0; i < t->rows; i++){
        for (j = 0; j < t->cols; j++){
            if (t->cell[i][j].display_text)
                free(t->cell[i][j].display_text);
        }
        free(t->cell[i]);
    }
    free(t->cell);
    free(t);
}
