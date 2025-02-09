#include "genre.h"

#include "../sql.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void sql_printGenre(tableGenre *g, int count){
    int i;
    for (i = 0; i < count; i++){
        printf("%d) %d - %s\n",
               i, g[i].id,
               g[i].genre == NULL ? "NULL" : g[i].genre);
    }
}

int sql_genreCallback(void *sql_table, int argc, char *argv[], char *col_name[]){
    sqlTable *st = (sqlTable*)sql_table;
    if (st && argc){
        if (st->count == 0){
            st->table = sql_initTable(&st->count, sizeof(tableGenre));
        } else {
            st->table = (void*)sql_addTable(st->table, &st->count, sizeof(tableGenre));
        }

        tableGenre *g = (tableGenre*)&((tableGenre*)st->table)[st->count - 1];

        int i;
        for (i = 0; i < argc; i++){
            switch(i){
                case GENRE_ID:
                    g->id = atoi(argv[i]);
                    break;
                case GENRE_NAME:
                    if (argv[i] != NULL)
                        g->genre = strdup(argv[i]);
                    else
                        g->genre = NULL;
                    break;
            }
        }
    }
    return 0;
}

void sql_freeTableGenre(tableGenre *g, int count){
    if (g){
        int i;
        for (i = 0; i < count; i++){
            if (g[i].genre)
                free(g[i].genre);
        }
        free(g);
    }
}
