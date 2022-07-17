#include <stdio.h>
#include <stdlib.h>

#include "shared.h"
#include "database.h"
#include "master_levels.h"

level *select_level(FILE *db, level *l, size_t sizeof_struct, int id) {
    if (id == -1) {
        int i = 0;
        while (fread(&l[i], sizeof_struct, 1, db)) {
            i++;
            l = realloc(l, sizeof_struct * (i + 1));
            if (l == NULL) {
                printf("Error allocating memory\n");
                exit(0);
            }
        }
        l[i].id = -1;
        return l;
    } else {
        int index = find_index(level_entity, id);
        if (index != -1) {
            fseek(db, index * sizeof_struct, SEEK_SET);
        } else {
            fseek(db, 0, SEEK_SET);
        }
        while (fread(l, sizeof_struct, 1, db) == 1) {
            if (l->id == id) {
                return l;
            }
        }
        free(l);
        return NULL;
    }
}

int delete_level(FILE *db, level *l, size_t sizeof_struct, int id) {
    l = select(db, level_entity, sizeof_struct, -1);
    FILE *replace = connect(MASTER_LEVELS_DB, "wb");
    int found = 0;
    fseek(replace, 0, SEEK_SET);
    for (int i = 0; l[i].id != -1; i++) {
        if (l[i].id == id) {
            found = 1;
            continue;
        } else {
            fwrite(&l[i], sizeof_struct, 1, replace);
        }
    }
    free(l);
    disconnect(replace);
    return found;
}

int insert_level(FILE *db, level *l, size_t sizeof_struct, level *entity) {
    fseek(db, 0, SEEK_END);
    int id = ftell(db) / sizeof_struct;
    entity->id = id + 1;
    fseek(db, 0, SEEK_SET);
    while (fread(l, sizeof_struct, 1, db) == 1) {
        if (l->id == entity->id) {
            free(entity);
            return 0;
        }
    }
    fseek(db, 0, SEEK_END);
    fwrite(entity, sizeof_struct, 1, db);
    free(entity);
    return 1;
}

int update_level(FILE *db, level *l, size_t sizeof_struct, level *entity, int id) {
    fseek(db, 0, SEEK_SET);
    while (fread(l, sizeof_struct, 1, db) == 1) {
        if (l->id == id) {
            entity->id = id;
            fseek(db, -sizeof_struct, SEEK_CUR);
            fwrite(entity, sizeof_struct, 1, db);
            free(entity);
            return 1;
        }
    }
    free(entity);
    return 0;
}
