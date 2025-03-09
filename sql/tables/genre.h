#ifndef _TABLE_GENRE_H
#define _TABLE_GENRE_H

#include <stdlib.h>

// Query to get Genre table data
#define TABLE_GENRE_SELECT "SELECT * FROM genre ORDER BY id;"

// Genre table columns enum
enum{
    GENRE_ID,
    GENRE_NAME
};

// Genre table struct
typedef struct{
    int id;
    char *genre;
}tableGenre;

/* For Debug: print genre struct */
void sql_printGenre(tableGenre *g, int count);

/*
    SQLite3 path callback
    > sqlite3_exec(db, TABLE_ANIME_SELECT, sql_animeCallback, (void*)table_genre, &sql_errmsg);

    sql_table is a pointer to a sqlTable struct containing a void pointer to a tableGenre struct.
*/
int sql_genreCallback(void *sql_table, int argc, char *argv[], char *col_name[]);

/*
    As data from SQLite3 is duplicated into the table struct,
    it needs to be free'd when done.
*/
void sql_freeTableGenre(tableGenre *g, int count);


#endif // _TABLE_GENRE_H
