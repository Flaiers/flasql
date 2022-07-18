#include <stdio.h>
#include <stdlib.h>

#include "shared.h"
#include "database.h"
#include "master_modules.h"

void *get_module(void *e, int i) {
    return &((module*)e)[i];
}

int get_module_id(void *e) {
    return ((module*)e)->id;
}

int set_module_id(void *e, int id) {
    ((module*)e)->id = id;
    return id;
}

module *select_module(FILE *db, module *m, size_t sizeof_struct, int id) {
    if (id == -1) {
        int i = 0;
        while (fread(&m[i], sizeof_struct, 1, db)) {
            i++;
            m = realloc(m, sizeof_struct * (i + 1));
            if (m == NULL) {
                printf("Error allocating memory\n");
                exit(0);
            }
        }
        m[i].id = -1;
        return m;
    } else {
        int index = find_index(module_entity, id);
        if (index != -1) {
            fseek(db, index * sizeof_struct, SEEK_SET);
        } else {
            fseek(db, 0, SEEK_SET);
        }
        while (fread(m, sizeof_struct, 1, db) == 1) {
            if (m->id == id) {
                return m;
            }
        }
        free(m);
        return NULL;
    }
}

int delete_module(FILE *db, module *m, size_t sizeof_struct, int id) {
    fseek(db, 0, SEEK_SET);
    while (fread(m, sizeof_struct, 1, db) == 1) {
        if (m->id == id) {
            m->deletion_flag = 1;
            fseek(db, -sizeof_struct, SEEK_CUR);
            fwrite(m, sizeof_struct, 1, db);
            free(m);
            return 1;
        }
    }
    return 0;
}

int insert_module(FILE *db, module *m, size_t sizeof_struct, module *entity) {
    fseek(db, 0, SEEK_END);
    int id = ftell(db) / sizeof_struct;
    entity->id = id;
    fseek(db, 0, SEEK_SET);
    while (fread(m, sizeof_struct, 1, db) == 1) {
        if (m->id == entity->id) {
            free(m);
            free(entity);
            return 0;
        }
    }
    fseek(db, 0, SEEK_END);
    fwrite(entity, sizeof_struct, 1, db);
    free(entity);
    return 1;
}

int update_module(FILE *db, module *m, size_t sizeof_struct, module *entity, int id) {
    fseek(db, 0, SEEK_SET);
    while (fread(m, sizeof_struct, 1, db) == 1) {
        if (m->id == id) {
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
