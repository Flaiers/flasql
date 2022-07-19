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

void *get_status_event(void *e, int i);
int get_status_event_id(void *e);
int set_status_event_id(void *e, int id);

status_event *select_status_event(FILE *db, status_event *s, int id);
int insert_status_event(FILE *db, status_event *s, status_event *data);
int update_status_event(FILE *db, status_event *s, status_event *data, int id);
int delete_status_event(FILE *db, status_event *s, int id);

#endif
