#ifndef _TABLE_ANIME_H
#define _TABLE_ANIME_H

// Query to get Anime table data
#define TABLE_ANIME_SELECT "SELECT * FROM anime ORDER BY name;"

// Anime table columns enum
enum{
    ANIME_ID,
    ANIME_NAME,
    ANIME_GENRE,
    ANIME_EPISODES,
    ANIME_WATCHED,
    ANIME_FINISHED
};

// Anime table struct
typedef struct{
    int id;
    char *name;
    char *genre;
    int episodes;
    int watched;
    int finished;
}tableAnime;

/* For Debug: print genre struct */
void sql_printAnime(tableAnime *a, int count);

/*
    SQLite3 path callback
    > sqlite3_exec(db, TABLE_ANIME_SELECT, sql_animeCallback, (void*)sql_table_anime, &sql_errmsg);

    sql_table is a pointer to a sqlTable struct containing a void pointer to a tableAnime struct.
*/
int sql_animeCallback(void *sql_table, int argc, char *argv[], char *col_name[]);

/*
    As data from SQLite3 is duplicated into the table struct,
    it needs to be free'd when done.
*/
void sql_freeTableAnime(tableAnime *a, int count);


#endif // _TABLE_ANIME_H
