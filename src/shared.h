#ifndef SHARED_H
#define SHARED_H

#undef MAX
#define MAX 100

#undef MASTER_LEVELS_IDX
#define MASTER_LEVELS_IDX "master_levels.idx"

#undef MASTER_MODULES_IDX
#define MASTER_MODULES_IDX "master_modules.idx"

#undef MASTER_STATUS_EVENTS_IDX
#define MASTER_STATUS_EVENTS_IDX "master_status_events.idx"

#include "master_levels.h"
#include "master_modules.h"
#include "master_status_events.h"

typedef enum entity {
    level_entity,
    module_entity,
    status_event_entity
} entity;

typedef struct index {
    int id;
    int pos;
} index;

void init_index(FILE *db, entity type, size_t sizeof_struct);
int find_index(entity type, int id);

void *select(FILE *db, entity type, size_t sizeof_struct, int id);
int insert(FILE *db, entity type, size_t sizeof_struct, void *entity);
int update(FILE *db, entity type, size_t sizeof_struct, void *entity, int id);
int delete(FILE *db, entity type, size_t sizeof_struct, int id);

module *select_active_modules(FILE *status_event_db, FILE *module_db);
module *select_modules_by_level_id(FILE *db, int level_id);
module *select_active_additional_modules(FILE *status_event_db, FILE *module_db);

int delete_modules_by_ids(FILE *db, int ids[MAX], int length);
int set_level_protection_flag(FILE *db, int id);
int set_module_protected_mode(FILE *module_db, FILE *status_event_db, int id);
int set_main_module_protected_mode(FILE *db);
int move_main_module_with_protection_flag(FILE *module_db, FILE *level_db);
int move_module_to_level_cell(FILE *db, int id, int level_id, int cell_id);

void turn_off_active_additional_modules(FILE *db);
void delete_active_additional_modules(FILE *db);

#endif
