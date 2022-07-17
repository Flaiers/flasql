#include <stdio.h>
#include <stdlib.h>

#include "database.h"

FILE *connect(const char *db_name, const char *mode) {
    char db_path[256];
    snprintf(db_path, sizeof(db_path), "%s/%s", DIR_DB, db_name);
    FILE *db = fopen(db_path, mode);
    if (db == NULL) {
        printf("Error db connection\n");
        exit(0);
    }
    return db;
}

void disconnect(FILE *db) {
    fclose(db);
}
