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

status_event *select_status_event(FILE *db, status_event *s, size_t sizeof_struct, int id) {
    if (id == -1) {
        int i = 0;
        while (fread(&s[i], sizeof_struct, 1, db)) {
            i++;
            s = realloc(s, sizeof_struct * (i + 1));
            if (s == NULL) {
                printf("Error allocating memory\n");
                exit(0);
            }
        }
        s[i].id = -1;
        return s;
    } else {
        int index = find_index(status_event_entity, id);
        if (index != -1) {
            fseek(db, index * sizeof_struct, SEEK_SET);
        } else {
            fseek(db, 0, SEEK_SET);
        }
        while (fread(s, sizeof_struct, 1, db) == 1) {
            if (s->id == id) {
                return s;
            }
        }
        free(s);
        return NULL;
    }
}

int delete_status_event(FILE *db, status_event *s, size_t sizeof_struct, int id) {
    s = select(db, status_event_entity, sizeof_struct, -1);
    FILE *replace = connect(MASTER_STATUS_EVENTS_DB, "wb");
    int found = 0;
    fseek(replace, 0, SEEK_SET);
    for (int i = 0; s[i].id != -1; i++) {
        if (s[i].id == id) {
            found = 1;
        } else {
            fwrite(&s[i], sizeof_struct, 1, replace);
        }
    }
    free(s);
    disconnect(replace);
    return found;
}

int insert_status_event(FILE *db, status_event *s, size_t sizeof_struct, status_event *entity) {
    fseek(db, 0, SEEK_END);
    int id = ftell(db) / sizeof_struct;
    entity->id = id;
    fseek(db, 0, SEEK_SET);
    while (fread(s, sizeof_struct, 1, db) == 1) {
        if (s->id == entity->id) {
            free(s);
            free(entity);
            return 0;
        }
    }
    fseek(db, 0, SEEK_END);
    fwrite(entity, sizeof_struct, 1, db);
    free(entity);
    return 1;
}

int update_status_event(FILE *db, status_event *s, size_t sizeof_struct, status_event *entity, int id) {
    fseek(db, 0, SEEK_SET);
    while (fread(s, sizeof_struct, 1, db) == 1) {
        if (s->id == id) {
            entity->id = id;
            fseek(db, -sizeof_struct, SEEK_CUR);
            fwrite(entity, sizeof_struct, 1, db);
            free(s);
            free(entity);
            return 1;
        }
    }
    free(entity);
    return 0;
}
