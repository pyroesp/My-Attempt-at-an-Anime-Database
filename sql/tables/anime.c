#include "anime.h"

#include "../sql.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void sql_printAnime(tableAnime *a, int count){
    int i;
    for (i = 0; i < count; i++){
        printf("%d) %d - %s - %s - %d - %d - %d\n",
               i, a[i].id,
               a[i].name == NULL ? "NULL" : a[i].name,
               a[i].genre== NULL ? "NULL" : a[i].genre,
               a[i].episodes, a[i].watched, a[i].finished);
    }
}

int sql_animeCallback(void *sql_table, int argc, char *argv[], char *col_name[]){
    int i;
    sqlTable *st = (sqlTable*)sql_table;
    tableAnime *a = NULL;
    if (st && argc){
        // allocate memory
        if (st->count == 0){
            st->table = sql_initTable(&st->count, sizeof(tableAnime));
        } else {
            st->table = (void*)sql_addTable(st->table, &st->count, sizeof(tableAnime));
        }
        // set newest element of table
        a = (tableAnime*)&((tableAnime*)st->table)[st->count - 1];
        for (i = 0; i < argc; i++){
            int *it = NULL;
            char *ct = NULL;

            switch(i){
                case ANIME_GENRE:
                case ANIME_NAME:
                case ANIME_EPISODES:
                case ANIME_WATCHED:
                case ANIME_FINISHED:
                    if (argv[i] != NULL)
                        ct = strdup(argv[i]);
                    break;
                case ANIME_ID:
                    it = &a->id;
                    break;
            }

            switch(i){
                case ANIME_ID:
                    (*it) = atoi(argv[i]);
                    break;
                case ANIME_EPISODES:
                    a->episodes = ct;
                    break;
                case ANIME_WATCHED:
                    a->watched = ct;
                    break;
                case ANIME_FINISHED:
                    a->finished = ct;
                    break;
                case ANIME_NAME:
                    a->name = ct;
                    break;
                case ANIME_GENRE:
                    a->genre = ct;
                    break;
            }
        }
    }
    return 0;
}

void sql_freeTableAnime(tableAnime *a, int count){
    if (a){
        int i;
        for (i = 0; i < count; i++){
            if (a[i].name)
                free(a[i].name);
            if (a[i].genre)
                free(a[i].genre);
            if (a[i].episodes)
                free(a[i].episodes);
            if (a[i].watched)
                free(a[i].watched);
            if (a[i].finished)
                free(a[i].finished);
        }
        free(a);
    }
}
