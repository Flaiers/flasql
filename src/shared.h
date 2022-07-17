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
    int index;
} index;

void init_index(FILE *db, entity type, size_t sizeof_struct);
int find_index(entity type, int id);

void *select(FILE *db, entity type, size_t sizeof_struct, int id);
int insert(FILE *db, entity type, size_t sizeof_struct, void *entity);
int update(FILE *db, entity type, size_t sizeof_struct, void *entity, int id);
int delete(FILE *db, entity type, size_t sizeof_struct, int id);

#endif
