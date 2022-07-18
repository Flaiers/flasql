#include <stdio.h>
#include <stdlib.h>

#include "shared.h"
#include "database.h"
#include "master_levels.h"
#include "master_modules.h"
#include "master_status_events.h"

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

void test_sel_levels() {
    FILE *db = connect(MASTER_LEVELS_DB, "rb+");
    level *l = malloc(sizeof(level));
    l = select(db, level_entity, sizeof(level), l, -1, get_level, get_level_id, set_level_id);
    if (l == NULL) {
        printf("Levels not found\n");
    } else {
        for (int i = 0; l[i].id != -1; i++) {
            output_level(&l[i]);
        }
    }
    disconnect(db);
    free(l);
}

void test_sel_modules() {
    FILE *db = connect(MASTER_MODULES_DB, "rb+");
    module *m = malloc(sizeof(module));
    m = select(db, module_entity, sizeof(module), m, -1, get_module, get_module_id, set_module_id);
    if (m == NULL) {
        printf("Modules not found\n");
    } else {
        for (int i = 0; m[i].id != -1; i++) {
            output_module(&m[i]);
        }
    }
    disconnect(db);
    free(m);
}

void test_sel_status_events() {
    FILE *db = connect(MASTER_STATUS_EVENTS_DB, "rb+");
    status_event *s = malloc(sizeof(status_event));
    s = select(db, status_event_entity, sizeof(status_event), s, -1, get_status_event, get_status_event_id, set_status_event_id);
    if (s == NULL) {
        printf("Status events not found\n");
    } else {
        for (int i = 0; s[i].id != -1; i++) {
            output_status_event(&s[i]);
        }
    }
    disconnect(db);
    free(s);
}

void test_sel_level() {
    FILE *db = connect(MASTER_LEVELS_DB, "rb+");
    level *l = malloc(sizeof(level));
    createidx(db, level_entity, sizeof(level), l, get_level_id);
    l = select(db, level_entity, sizeof(level), l, 1, get_level, get_level_id, set_level_id);
    if (l == NULL) {
        printf("Level not found\n");
    } else {
        output_level(l);
    }
    disconnect(db);
    free(l);
}

void test_sel_module() {
    FILE *db = connect(MASTER_MODULES_DB, "rb+");
    module *m = malloc(sizeof(module));
    createidx(db, module_entity, sizeof(module), m, get_module_id);
    m = select(db, module_entity, sizeof(module), m, 1, get_module, get_module_id, set_module_id);
    if (m == NULL) {
        printf("Module not found\n");
    } else {
        output_module(m);
    }
    disconnect(db);
    free(m);
}

void test_sel_status_event() {
    FILE *db = connect(MASTER_STATUS_EVENTS_DB, "rb+");
    status_event *s = malloc(sizeof(status_event));
    createidx(db, status_event_entity, sizeof(status_event), s, get_status_event_id);
    s = select(db, status_event_entity, sizeof(status_event), s, 1, get_status_event, get_status_event_id, set_status_event_id);
    if (s == NULL) {
        printf("Status event not found\n");
    } else {
        output_status_event(s);
    }
    disconnect(db);
    free(s);
}

void test_ins_level() {
    FILE *db = connect(MASTER_LEVELS_DB, "rb+");
    level *l = malloc(sizeof(level));
    level data = {
        .id = 1,
        .cell_amount = 10,
        .protection_flag = 1
    };
    int status = insert(db, sizeof(level), l, &data, get_level_id, set_level_id);
    if (status == 0) {
        printf("Level already exists\n");
    } else {
        printf("Level inserted\n");
    }
    disconnect(db);
    free(l);
}

void test_ins_module() {
    FILE *db = connect(MASTER_MODULES_DB, "rb+");
    module *m = malloc(sizeof(module));
    module data = {
        .name = "Some module 99",
        .level_id = 1,
        .cell_id = 1,
        .deletion_flag = 0
    };
    int status = insert(db, sizeof(module), m, &data, get_module_id, set_module_id);
    if (status == 0) {
        printf("Module already exists\n");
    } else {
        printf("Module inserted\n");
    }
    disconnect(db);
    free(m);
}

void test_ins_status_event() {
    FILE *db = connect(MASTER_STATUS_EVENTS_DB, "rb+");
    status_event *s = malloc(sizeof(status_event));
    status_event data = {
        .module_id = 1,
        .module_status = 1,
        .status_change_date = "18.07.2022",
        .status_change_time = "02:07:10"
    };
    int status = insert(db, sizeof(status_event), s, &data, get_status_event_id, set_status_event_id);
    if (status == 0) {
        printf("Status event already exists\n");
    } else {
        printf("Status event inserted\n");
    }
    disconnect(db);
    free(s);
}

void test_upd_level() {
    FILE *db = connect(MASTER_LEVELS_DB, "rb+");
    level *l = malloc(sizeof(level));
    createidx(db, level_entity, sizeof(level), l, get_level_id);
    level data = {
        .cell_amount = 10,
        .protection_flag = 1
    };
    int status = update(db, level_entity, sizeof(level), l, &data, 1, get_level_id, set_level_id);
    if (status == 0) {
        printf("Level not found\n");
    } else {
        printf("Level updated\n");
    }
    disconnect(db);
    free(l);
}

void test_upd_module() {
    FILE *db = connect(MASTER_MODULES_DB, "rb+");
    module *m = malloc(sizeof(module));
    createidx(db, module_entity, sizeof(module), m, get_module_id);
    module data = {
        .name = "Some module 1",
        .level_id = 10,
        .cell_id = 10,
        .deletion_flag = 1
    };
    int status = update(db, module_entity, sizeof(module), m, &data, 1, get_module_id, set_module_id);
    if (status == 0) {
        printf("Module not found\n");
    } else {
        printf("Module updated\n");
    }
    disconnect(db);
    free(m);
}

void test_upd_status_event() {
    FILE *db = connect(MASTER_STATUS_EVENTS_DB, "rb+");
    status_event *s = malloc(sizeof(status_event));
    createidx(db, status_event_entity, sizeof(status_event), s, get_status_event_id);
    status_event data = {
        .module_id = 10,
        .module_status = 1,
        .status_change_date = "18.07.2022",
        .status_change_time = "02:07:10"
    };
    int status = update(db, status_event_entity, sizeof(status_event), s, &data, 1, get_status_event_id, set_status_event_id);
    if (status == 0) {
        printf("Status event not found\n");
    } else {
        printf("Status event updated\n");
    }
    disconnect(db);
    free(s);
}

void test_del_level() {
    FILE *db = connect(MASTER_LEVELS_DB, "rb+");
    level *l = malloc(sizeof(level));
    int status = delete(db, level_entity, sizeof(level), l, 1, get_level, get_level_id, set_level_id);
    if (status == 0) {
        printf("Level not found\n");
    } else {
        printf("Level deleted\n");
    }
    disconnect(db);
    free(l);
}

void test_del_module() {
    FILE *db = connect(MASTER_MODULES_DB, "rb+");
    module *m = malloc(sizeof(module));
    int status = delete(db, module_entity, sizeof(module), m, 1, get_module, get_module_id, set_module_id);
    if (status == 0) {
        printf("Module not found\n");
    } else {
        printf("Module deleted\n");
    }
    disconnect(db);
    free(m);
}

void test_del_status_event() {
    FILE *db = connect(MASTER_STATUS_EVENTS_DB, "rb+");
    status_event *s = malloc(sizeof(status_event));
    int status = delete(db, status_event_entity, sizeof(status_event), s, 1, get_status_event, get_status_event_id, set_status_event_id);
    if (status == 0) {
        printf("Status event not found\n");
    } else {
        printf("Status event deleted\n");
    }
    disconnect(db);
    free(s);
}

int main() {
    test_sel_levels();
    test_sel_modules();
    test_sel_status_events();

    test_sel_level();
    test_sel_module();
    test_sel_status_event();

    test_ins_level();
    test_ins_module();
    test_ins_status_event();

    test_upd_level();
    test_upd_module();
    test_upd_status_event();

    test_del_level();
    test_del_module();
    test_del_status_event();
    return 0;
}
