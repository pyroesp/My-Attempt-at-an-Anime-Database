#include "sql.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Test SQLite3 callback
int sql_callback(void *not_used, int argc, char **argv, char **col_name){
    printf("argc = %d\n", argc);
    for (int i = 0; i < argc; i++)
        printf("(%d)%16s | ", i, col_name[i]);
    printf("\n");
    return 0;
}


int sql_operation(sqlite3 *db, sqlOpList s){
    #define SQL_QUERY_SIZE 512
    char query[SQL_QUERY_SIZE];
    switch(s.type){
        case SQL_OP_DELETE:
            snprintf(query, SQL_QUERY_SIZE, "DELETE FROM %s WHERE id=%d;", s.table, s.id);
            break;
        case SQL_OP_UPDATE:
            snprintf(query, SQL_QUERY_SIZE, "UPDATE FROM %s SET %s=%s WHERE id=%d;", s.table, s.column, s.new_value, s.id);
            break;
        case SQL_OP_INSERT:
            snprintf(query, SQL_QUERY_SIZE, "INSERT INTO %s(%s) VALUES('%s');", s.table, s.column, s.new_value);
            break;
    }

    return sqlite3_exec(db, query, 0, 0, 0);
}


void* sql_initTable(int *count, int size_of_data){
    void *table;
    table = (void*)calloc(1, size_of_data);
    (*count) = 1;
    return table;
}

void* sql_addTable(void *table, int *count, int size_of_data){
    (*count)++;
    table = (void*)realloc(table, (*count) * size_of_data);
    // get new element by casting it to char and adding the count*sizeofdata to it
    char *new_element = (char*)table + ((*count) - 1) * size_of_data;
    memset(new_element, 0, size_of_data);
    return table;
}
