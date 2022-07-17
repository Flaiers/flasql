#ifndef MASTER_STATUS_EVENTS_H
#define MASTER_STATUS_EVENTS_H

#undef MASTER_STATUS_EVENTS_DB
#define MASTER_STATUS_EVENTS_DB "master_status_events.db"

typedef struct status_event {
    int id;
    int module_id;
    int module_status;
    char status_change_date[11];
    char status_change_time[9];
} status_event;

status_event *select_status_event(FILE *db, status_event *s, size_t sizeof_struct, int id);
int delete_status_event(FILE *db, status_event *s, size_t sizeof_struct, int id);
int insert_status_event(FILE *db, status_event *s, size_t sizeof_struct, status_event *entity);
int update_status_event(FILE *db, status_event *s, size_t sizeof_struct, status_event *entity, int id);

#endif
