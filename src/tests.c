#include <stdio.h>
#include <stdlib.h>

#include "shared.h"
#include "database.h"

int get_level_id(void *e) {
    return ((level*)e)->id;
}

int get_module_id(void *e) {
    return ((module*)e)->id;
}

int get_status_event_id(void *e) {
    return ((status_event*)e)->id;
}

void output_level(level *l) {
    printf("%d %d %d\n", l->id,
                         l->cell_amount,
                         l->protection_flag);
}

void output_module(module *m) {
    printf("%d %s %d %d %d\n", m->id,
                               m->name,
                               m->level_id,
                               m->cell_id,
                               m->deletion_flag);
}

void output_status_event(status_event *s) {
    printf("%d %d %d %s %s\n", s->id,
                               s->module_id,
                               s->module_status,
                               s->status_change_date,
                               s->status_change_time);
}

void iindex(FILE *db, entity type, void *data, size_t sizeof_entity, int (*get_id)(void *)) {
    int i = 0;
    index indx;
    FILE *idx = NULL;
    if (type == level_entity) {
        idx = connect(MASTER_LEVELS_IDX, "wb");
    } else if (type == module_entity) {
        idx = connect(MASTER_MODULES_IDX, "wb");
    } else if (type == status_event_entity) {
        idx = connect(MASTER_STATUS_EVENTS_IDX, "wb");
    }
    fseek(db, 0, SEEK_SET);
    while (fread(data, sizeof_entity, 1, db) == 1) {
        indx.id = get_id(data);
        indx.index = i;
        fwrite(&indx, sizeof(index), 1, idx);
        i++;
    }
    disconnect(idx);
}

int findex(entity type, int id) {
    int i = -1;
    index indx;
    FILE *idx = NULL;
    if (type == level_entity) {
        idx = connect(MASTER_LEVELS_IDX, "rb+");
    } else if (type == module_entity) {
        idx = connect(MASTER_MODULES_IDX, "rb+");
    } else if (type == status_event_entity) {
        idx = connect(MASTER_STATUS_EVENTS_IDX, "rb+");
    }
    fseek(idx, 0, SEEK_SET);
    while (fread(&indx, sizeof(index), 1, idx) == 1) {
        if (indx.id == id) {
            i = indx.index;
            break;
        }
    }
    disconnect(idx);
    return i;
}

void *sel(FILE *db, entity type, void *data, size_t sizeof_entity, int (*get_id)(void *), int id) {
    int index = findex(type, id);
    if (index != -1) {
        fseek(db, index * sizeof_entity, SEEK_SET);
        fread(data, sizeof_entity, 1, db);
        return data;
    } else {
        fseek(db, 0, SEEK_SET);
        while (fread(data, sizeof_entity, 1, db) == 1) {
            if (get_id(data) == id) {
                return data;
            }
        }
    }
    return NULL;
}

void test_levels() {
    FILE *db = connect(MASTER_LEVELS_DB, "rb+");
    level *l = malloc(sizeof(level));
    iindex(db, level_entity, l, sizeof(level), get_level_id);
    l = sel(db, level_entity, l, sizeof(level), get_level_id, 1);
    if (l == NULL) {
        printf("Level not found\n");
    } else {
        output_level(l);
    }
    disconnect(db);
    free(l);
}

void test_modules() {
    FILE *db = connect(MASTER_MODULES_DB, "rb+");
    module *m = malloc(sizeof(module));
    iindex(db, module_entity, m, sizeof(module), get_module_id);
    m = sel(db, module_entity, m, sizeof(module), get_module_id, 1);
    if (m == NULL) {
        printf("Module not found\n");
    } else {
        output_module(m);
    }
    disconnect(db);
    free(m);
}

void test_status_events() {
    FILE *db = connect(MASTER_STATUS_EVENTS_DB, "rb+");
    status_event *s = malloc(sizeof(status_event));
    iindex(db, status_event_entity, s, sizeof(status_event), get_status_event_id);
    s = sel(db, status_event_entity, s, sizeof(status_event), get_status_event_id, 1);
    if (s == NULL) {
        printf("Status event not found\n");
    } else {
        output_status_event(s);
    }
    disconnect(db);
    free(s);
}

int main() {
    test_levels();
    test_modules();
    test_status_events();
    return 0;
}
