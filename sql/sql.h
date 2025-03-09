#ifndef _SQL_H
#define _SQL_H

#include "sqlite/sqlite3.h"

// Default path to anime database
#define SQL_DATABASE_PATH "./db/anime.db"

// SQL Operation Types enum
typedef enum{
    SQL_OP_INSERT,
    SQL_OP_UPDATE,
    SQL_OP_DELETE
}sqlOpType;

// Struct containing list of operations to execute
typedef struct{
    sqlOpType type;
    char table[100];
    char column[100];
    char new_value[100];
    int id;
}sqlOpList;

// Standard SQL Table structure to pass in sqlite3_exec
typedef struct{
    int count;
    void *table;
}sqlTable;

/* Read operation list and execute INSERT/UPDATE/DELETE queries */
int sql_operation(sqlite3 *db, sqlOpList s);

/* Initialize void table structure used by table code */
void* sql_initTable(int *count, int size_of_data);

/* Add element to table array used by table code */
void* sql_addTable(void *table, int *count, int size_of_data);


#endif // _SQL_H
