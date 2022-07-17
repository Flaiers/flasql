#include <stdio.h>
#include <stdlib.h>

#include "shared.h"
#include "database.h"

void *convert(table t) {
    if (t.type == level_entity) {
        return (level*)t.value;
    } else if (t.type == module_entity) {
        return (module*)t.value;
    } else if (t.type == status_event_entity) {
        return (status_event*)t.value;
    }
}

int convert_to_id(table t) {
    if (t.type == level_entity) {
        return ((level*)t.value)->id;
    } else if (t.type == module_entity) {
        return ((module*)t.value)->id;
    } else if (t.type == status_event_entity) {
        return ((status_event*)t.value)->id;
    }
}

void iindex(FILE *db, table t) {
    int i = 0;
    index indx;
    FILE *idx = NULL;
    if (t.type == level_entity) {
        idx = connect(MASTER_LEVELS_IDX, "wb");
    } else if (t.type == module_entity) {
        idx = connect(MASTER_MODULES_IDX, "wb");
    } else if (t.type == status_event_entity) {
        idx = connect(MASTER_STATUS_EVENTS_IDX, "wb");
    }
    fseek(db, 0, SEEK_SET);
    while (fread(t.value, t.size, 1, db) == 1) {
        indx.id = convert_to_id(t);
        indx.index = i;
        fwrite(&indx, sizeof(index), 1, idx);
        i++;
    }
    disconnect(idx);
}

int findex(table t, int id) {
    int i = -1;
    index indx;
    FILE *idx = NULL;
    if (t.type == level_entity) {
        idx = connect(MASTER_LEVELS_IDX, "rb+");
    } else if (t.type == module_entity) {
        idx = connect(MASTER_MODULES_IDX, "rb+");
    } else if (t.type == status_event_entity) {
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

void *sel(FILE *db, table t, int id) {
    int index = findex(t, id);
    if (index != -1) {
        fseek(db, index * t.size, SEEK_SET);
        fread(convert(t), t.size, 1, db);
        return t.value;
    } else {
        fseek(db, 0, SEEK_SET);
        while (fread(convert(t), t.size, 1, db) == 1) {
            if (convert_to_id(t) == id) {
                return t.value;
            }
        }
    }
    return NULL;
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

void test_levels() {
    FILE *db = connect(MASTER_LEVELS_DB, "rb+");
    level *l = malloc(sizeof(level));
    table t = {
        .value = l,
        .type = level_entity,
        .size = sizeof(level)
    };
    iindex(db, t);
    l = sel(db, t, 1);
    if (l != NULL) {
        output_level(l);
    }
    disconnect(db);
    free(l);
}

void test_modules() {
    FILE *db = connect(MASTER_MODULES_DB, "rb+");
    module *m = malloc(sizeof(module));
    table t = {
        .value = m,
        .type = module_entity,
        .size = sizeof(module)
    };
    iindex(db, t);
    m = sel(db, t, 0);
    if (m != NULL) {
        output_module(m);
    }
    disconnect(db);
    free(m);
}

void test_status_events() {
    FILE *db = connect(MASTER_STATUS_EVENTS_DB, "rb+");
    status_event *s = malloc(sizeof(status_event));
    table t = {
        .value = s,
        .type = status_event_entity,
        .size = sizeof(status_event)
    };
    iindex(db, t);
    s = sel(db, t, 0);
    if (s != NULL) {
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
