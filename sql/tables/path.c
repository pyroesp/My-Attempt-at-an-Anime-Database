#include "path.h"

#include "../sql.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void sql_printPath(tablePath *p, int count){
    int i;
    for (i = 0; i < count; i++){
        printf("%d) %d - %s\n",
               i, p[i].id,
               p[i].path == NULL ? "NULL" : p[i].path);
    }
}

int sql_pathCallback(void *sql_table, int argc, char *argv[], char *col_name[]){
    sqlTable *st = (sqlTable*)sql_table;
    if (st && argc){
        if (st->count == 0){
            st->table = sql_initTable(&st->count, sizeof(tablePath));
        } else {
            st->table = (void*)sql_addTable(st->table, &st->count, sizeof(tablePath));
        }

        tablePath *p = (tablePath*)&((tablePath*)st->table)[st->count - 1];

        int i;
        for (i = 0; i < argc; i++){
            switch(i){
                case PATH_ID:
                    p->id = atoi(argv[i]);
                    break;
                case PATH_NAME:
                    if (argv[i] != NULL)
                        p->path = strdup(argv[i]);
                    else
                        p->path = NULL;
                    break;
            }
        }
    }
    return 0;
}

void sql_freeTablePath(tablePath *p, int count){
    if (p){
        int i;
        for (i = 0; i < count; i++){
            if (p[i].path)
                free(p[i].path);
        }
        free(p);
    }
}
