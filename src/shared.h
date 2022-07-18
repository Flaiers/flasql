#ifndef SHARED_H
#define SHARED_H

#undef MASTER_LEVELS_IDX
#define MASTER_LEVELS_IDX "master_levels.idx"

#undef MASTER_MODULES_IDX
#define MASTER_MODULES_IDX "master_modules.idx"

#undef MASTER_STATUS_EVENTS_IDX
#define MASTER_STATUS_EVENTS_IDX "master_status_events.idx"

typedef enum entity {
    level_entity,
    module_entity,
    status_event_entity
} entity;

struct index {
    int id;
    int index;
};

int getidx(entity type, int id);
void createidx(FILE *db, entity type, size_t sizeof_entity, void *e, int (*getid)(void *));

void *select(FILE *db, entity type, size_t sizeof_entity, void *e, int id, void *(*get)(void *, int), int (*getid)(void *), int (*setid)(void *, int));
int insert(FILE *db, size_t sizeof_entity, void *e, void *data, int (*getid)(void *), int (*setid)(void *, int));
int update(FILE *db, entity type, size_t sizeof_entity, void *e, void *data, int id, int (*getid)(void *), int (*setid)(void *, int));
int delete(FILE *db, entity type, size_t sizeof_entity, void *e, int id, void *(*get)(void *, int), int (*getid)(void *), int (*setid)(void *, int));

#endif
