#include <stdio.h>
#include <stdlib.h>

#include "shared.h"
#include "database.h"

int get_level_id(void *e) {
    return ((level*)e)->id;
}

int set_level_id(void *e, int id) {
    ((level*)e)->id = id;
    return id;
}

int get_module_id(void *e) {
    return ((module*)e)->id;
}

int set_module_id(void *e, int id) {
    ((module*)e)->id = id;
    return id;
}

int get_status_event_id(void *e) {
    return ((status_event*)e)->id;
}

int set_status_event_id(void *e, int id) {
    ((status_event*)e)->id = id;
    return id;
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

void iindex(FILE *db, entity type, size_t sizeof_entity, void *e, int (*get_id)(void *)) {
    int i = 0;
    struct index indx;
    FILE *idx = NULL;
    if (type == level_entity) {
        idx = connect(MASTER_LEVELS_IDX, "wb");
    } else if (type == module_entity) {
        idx = connect(MASTER_MODULES_IDX, "wb");
    } else if (type == status_event_entity) {
        idx = connect(MASTER_STATUS_EVENTS_IDX, "wb");
    }
    fseek(db, 0, SEEK_SET);
    while (fread(e, sizeof_entity, 1, db) == 1) {
        indx.id = get_id(e);
        indx.index = i;
        fwrite(&indx, sizeof(struct index), 1, idx);
        i++;
    }
    disconnect(idx);
}

int findex(entity type, int id) {
    int i = -1;
    struct index indx;
    FILE *idx = NULL;
    if (type == level_entity) {
        idx = connect(MASTER_LEVELS_IDX, "rb+");
    } else if (type == module_entity) {
        idx = connect(MASTER_MODULES_IDX, "rb+");
    } else if (type == status_event_entity) {
        idx = connect(MASTER_STATUS_EVENTS_IDX, "rb+");
    }
    fseek(idx, 0, SEEK_SET);
    while (fread(&indx, sizeof(struct index), 1, idx) == 1) {
        if (indx.id == id) {
            i = indx.index;
            break;
        }
    }
    disconnect(idx);
    return i;
}

void *sel(FILE *db, entity type, size_t sizeof_entity, void *e, int id, int (*get_id)(void *)) {
    int index = findex(type, id);
    if (index != -1) {
        fseek(db, index * sizeof_entity, SEEK_SET);
        fread(e, sizeof_entity, 1, db);
        return e;
    } else {
        fseek(db, 0, SEEK_SET);
        while (fread(e, sizeof_entity, 1, db) == 1) {
            if (get_id(e) == id) {
                return e;
            }
        }
    }
    return NULL;
}

int ins(FILE *db, entity type, size_t sizeof_entity, void *e, void *data, int (*get_id)(void *), int (*set_id)(void *, int)) {
    fseek(db, 0, SEEK_SET);
    fread(e, sizeof_entity, 1, db);

    fseek(db, 0, SEEK_END);
    set_id(data, (ftell(db) / sizeof_entity) + get_id(e));
    fwrite(data, sizeof_entity, 1, db);
    return 1;
}

int upd(FILE *db, entity type, size_t sizeof_entity, void *e, void *data, int id, int (*get_id)(void *), int (*set_id)(void *, int)) {
    set_id(data, id);
    int index = findex(type, id);
    if (index != -1) {
        fseek(db, index * sizeof_entity, SEEK_SET);
        fwrite(data, sizeof_entity, 1, db);
        return 1;
    } else {
        fseek(db, 0, SEEK_SET);
        while (fread(e, sizeof_entity, 1, db) == 1) {
            if (get_id(e) == id) {
                fseek(db, -sizeof_entity, SEEK_CUR);
                fwrite(data, sizeof_entity, 1, db);
                return 1;
            }
        }
    }
    return 0;
}

void test_sel_levels() {
    FILE *db = connect(MASTER_LEVELS_DB, "rb+");
    level *l = malloc(sizeof(level));
    iindex(db, level_entity, sizeof(level), l, get_level_id);
    l = sel(db, level_entity, sizeof(level), l, 1, get_level_id);
    if (l == NULL) {
        printf("Level not found\n");
    } else {
        output_level(l);
    }
    disconnect(db);
    free(l);
}

void test_sel_modules() {
    FILE *db = connect(MASTER_MODULES_DB, "rb+");
    module *m = malloc(sizeof(module));
    iindex(db, module_entity, sizeof(module), m, get_module_id);
    m = sel(db, module_entity, sizeof(module), m, 1, get_module_id);
    if (m == NULL) {
        printf("Module not found\n");
    } else {
        output_module(m);
    }
    disconnect(db);
    free(m);
}

void test_sel_status_events() {
    FILE *db = connect(MASTER_STATUS_EVENTS_DB, "rb+");
    status_event *s = malloc(sizeof(status_event));
    iindex(db, status_event_entity, sizeof(status_event), s, get_status_event_id);
    s = sel(db, status_event_entity, sizeof(status_event), s, 1, get_status_event_id);
    if (s == NULL) {
        printf("Status event not found\n");
    } else {
        output_status_event(s);
    }
    disconnect(db);
    free(s);
}

void test_ins_levels() {
    FILE *db = connect(MASTER_LEVELS_DB, "rb+");
    level *l = malloc(sizeof(level));
    level data = {
        .id = 1,
        .cell_amount = 10,
        .protection_flag = 1
    };
    int status = ins(db, level_entity, sizeof(level), l, &data, get_level_id, set_level_id);
    if (status == 0) {
        printf("Level already exists\n");
    } else {
        printf("Level inserted\n");
    }
    disconnect(db);
    free(l);
}

void test_ins_modules() {
    FILE *db = connect(MASTER_MODULES_DB, "rb+");
    module *m = malloc(sizeof(module));
    module data = {
        .name = "Some module 99",
        .level_id = 1,
        .cell_id = 1,
        .deletion_flag = 0
    };
    int status = ins(db, module_entity, sizeof(module), m, &data, get_module_id, set_module_id);
    if (status == 0) {
        printf("Module already exists\n");
    } else {
        printf("Module inserted\n");
    }
    disconnect(db);
    free(m);
}

void test_ins_status_events() {
    FILE *db = connect(MASTER_STATUS_EVENTS_DB, "rb+");
    status_event *s = malloc(sizeof(status_event));
    status_event data = {
        .module_id = 1,
        .module_status = 1,
        .status_change_date = "18.07.2022",
        .status_change_time = "02:07:10"
    };
    int status = ins(db, status_event_entity, sizeof(status_event), s, &data, get_status_event_id, set_status_event_id);
    if (status == 0) {
        printf("Status event already exists\n");
    } else {
        printf("Status event inserted\n");
    }
    disconnect(db);
    free(s);
}

void test_upd_levels() {
    FILE *db = connect(MASTER_LEVELS_DB, "rb+");
    level *l = malloc(sizeof(level));
    level data = {
        .cell_amount = 10,
        .protection_flag = 1
    };
    int status = upd(db, level_entity, sizeof(level), l, &data, 1, get_level_id, set_level_id);
    if (status == 0) {
        printf("Level not found\n");
    } else {
        printf("Level updated\n");
    }
    disconnect(db);
    free(l);
}

void test_upd_modules() {
    FILE *db = connect(MASTER_MODULES_DB, "rb+");
    module *m = malloc(sizeof(module));
    module data = {
        .name = "Some module 1",
        .level_id = 10,
        .cell_id = 10,
        .deletion_flag = 1
    };
    int status = upd(db, module_entity, sizeof(module), m, &data, 1, get_module_id, set_module_id);
    if (status == 0) {
        printf("Module not found\n");
    } else {
        printf("Module updated\n");
    }
    disconnect(db);
    free(m);
}

void test_upd_status_events() {
    FILE *db = connect(MASTER_STATUS_EVENTS_DB, "rb+");
    status_event *s = malloc(sizeof(status_event));
    status_event data = {
        .module_id = 10,
        .module_status = 1,
        .status_change_date = "18.07.2022",
        .status_change_time = "02:07:10"
    };
    int status = upd(db, status_event_entity, sizeof(status_event), s, &data, 1, get_status_event_id, set_status_event_id);
    if (status == 0) {
        printf("Status event not found\n");
    } else {
        printf("Status event updated\n");
    }
    disconnect(db);
    free(s);
}

int main() {
    test_sel_levels();
    test_sel_modules();
    test_sel_status_events();

    // test_ins_levels();
    // test_ins_modules();
    // test_ins_status_events();

    test_upd_levels();
    test_upd_modules();
    test_upd_status_events();
    return 0;
}
