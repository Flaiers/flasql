#include <stdio.h>
#include <stdlib.h>

#include "database.h"

FILE *connect(const char *db_name, const char *mode) {
    char db_path[256];
    snprintf(db_path, sizeof(db_path), "%s/%s", DIR_DB, db_name);
    return fopen(db_path, mode);
}

void disconnect(FILE *db) {
    if (db != NULL) {
        fclose(db);
    }
}
