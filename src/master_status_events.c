#include <stdio.h>
#include <stdlib.h>

#include "shared.h"
#include "database.h"
#include "master_status_events.h"

void *get_status_event(void *e, int i) {
    return &((status_event*)e)[i];
}

int get_status_event_id(void *e) {
    return ((status_event*)e)->id;
}

int set_status_event_id(void *e, int id) {
    ((status_event*)e)->id = id;
    return id;
}

status_event *select_status_event(FILE *db, status_event *s, int id) {
    return select(db, status_event_entity, sizeof(status_event), s, id, get_status_event, get_status_event_id, set_status_event_id);
}

int insert_status_event(FILE *db, status_event *s, status_event *data) {
    return insert(db, sizeof(status_event), s, data, get_status_event_id, set_status_event_id);
}

int update_status_event(FILE *db, status_event *s, status_event *data, int id) {
    createidx(db, status_event_entity, sizeof(status_event), s, get_status_event_id);
    return update(db, status_event_entity, sizeof(status_event), s, data, id, get_status_event_id, set_status_event_id);
}

int delete_status_event(FILE *db, status_event *s, int id) {
    return delete(db, status_event_entity, sizeof(status_event), s, id, get_status_event, get_status_event_id, set_status_event_id);
}
