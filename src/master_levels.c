#include <stdio.h>
#include <stdlib.h>

#include "shared.h"
#include "database.h"
#include "master_levels.h"

void *get_level(void *e, int i) {
    return &((level*)e)[i];
}

int get_level_id(void *e) {
    return ((level*)e)->id;
}

int set_level_id(void *e, int id) {
    ((level*)e)->id = id;
    return id;
}

level *select_level(FILE *db, level *l, int id) {
    return select(db, level_entity, sizeof(level), l, id, get_level, get_level_id, set_level_id);
}

int insert_level(FILE *db, level *l, level *data) {
    return insert(db, sizeof(level), l, data, get_level_id, set_level_id);
}

int update_level(FILE *db, level *l, level *data, int id) {
    createidx(db, level_entity, sizeof(level), l, get_level_id);
    return update(db, level_entity, sizeof(level), l, data, id, get_level_id, set_level_id);
}

int delete_level(FILE *db, level *l, int id) {
    return delete(db, level_entity, sizeof(level), l, id, get_level, get_level_id, set_level_id);
}
