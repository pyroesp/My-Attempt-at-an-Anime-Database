#ifndef _TABLE_PATH_H
#define _TABLE_PATH_H

#include <stdlib.h>

// Query to get Path table data
#define TABLE_PATH_SELECT "SELECT * FROM path ORDER BY id;"

// Path table columns enum
enum{
    PATH_ID,
    PATH_NAME
};

// Path table struct
typedef struct{
    int id;
    char *path;
}tablePath;

/* For Debug: print path struct */
void sql_printPath(tablePath *p, int count);

/*
    SQLite3 path callback
    > sqlite3_exec(db, TABLE_PATH_SELECT, sql_pathCallback, (void*)table_path, &sql_errmsg);

    sql_table is a pointer to a sqlTable struct containing a void pointer to a tablePath struct.
*/
int sql_pathCallback(void *sql_table, int argc, char *argv[], char *col_name[]);

/*
    As data from SQLite3 is duplicated into the table struct,
    it needs to be free'd when done.
*/
void sql_freeTablePath(tablePath *p, int count);


#endif // _TABLE_PATH_H
