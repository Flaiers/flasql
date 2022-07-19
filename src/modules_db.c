#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "modules_db.h"

int main(void) {
    system("clear");
    int table_number = input_table();
    if (table_number == -1) {
        return 0;
    }
    int operation_number = input_operation();
    if (operation_number == -1) {
        return 0;
    }
    execute_query(table_number, operation_number);
    return 0;
}

int input_table() {
    char c;
    int number;
    printf("Please choose a table:\n"
           "    1. Modules\n"
           "    2. Levels\n"
           "    3. Status events\n"
           ">> ");
    if (!scanf("%d%c", &number, &c) || number < 1 || number > 3 || c != '\n') {
        return -1;
    }
    return number;
}

int input_operation() {
    char c;
    int number;
    printf("Please choose one operation:\n"
           "    1. SELECT\n"
           "    2. INSERT\n"
           "    3. UPDATE\n"
           "    4. DELETE\n"
           ">> ");
    if (!scanf("%d%c", &number, &c) || number < 1 || number > 4 || c != '\n') {
        return -1;
    }
    return number;
}

int input_id(const char *type) {
    char c;
    int id;
    printf("Enter the %s id:\n"
           ">> ", type);
    if (!scanf("%d%c", &id, &c) || c != '\n') {
        return -1;
    }
    return id;
}

int input_selected_id() {
    char c;
    int id;
    printf("Enter the selected id or leave empty to output all of them:\n"
           ">> ");
    if (!scanf("%d%c", &id, &c) || c != '\n') {
        id = -1;
    }
    return id;
}

module *input_module(module *m) {
    if (m == NULL) {
        return NULL;
    }
    char c;
    printf("Enter the attributes for the module:\n"
           ">> name: ");

    if (!scanf("%29s%c", m->name, &c) || c != '\n') {
        return NULL;
    }

    printf(">> level_id: ");
    if (!scanf("%d%c", &m->level_id, &c) || c != '\n') {
        return NULL;
    }

    printf(">> cell_id: ");
    if (!scanf("%d%c", &m->cell_id, &c) || c != '\n') {
        return NULL;
    }

    printf(">> deletion_flag: ");
    if (!scanf("%d%c", &m->deletion_flag, &c) || c != '\n') {
        return NULL;
    }
    return m;
}

void output_module(module *m) {
    printf("%d %s %d %d %d\n", m->id,
                               m->name,
                               m->level_id,
                               m->cell_id,
                               m->deletion_flag);
}

level *input_level(level *l) {
    if (l == NULL) {
        return NULL;
    }
    char c;
    printf("Enter the attributes for the level:\n"
           ">> cell_amount: ");
    if (!scanf("%d%c", &l->cell_amount, &c) || c != '\n') {
        return NULL;
    }

    printf(">> protection_flag: ");
    if (!scanf("%d%c", &l->protection_flag, &c) || c != '\n') {
        return NULL;
    }
    return l;
}

void output_level(level *l) {
    printf("%d %d %d\n", l->id,
                         l->cell_amount,
                         l->protection_flag);
}

status_event *input_status_event(status_event *s) {
    struct tm *t = malloc(sizeof(struct tm));
    if (s == NULL) {
        return NULL;
    }
    char c;
    printf("Enter the attributes for the status event:\n"
           ">> module_id: ");
    if (!scanf("%d%c", &s->module_id, &c) || c != '\n') {
        free(t);
        return NULL;
    }

    printf(">> module_status: ");
    if (!scanf("%d%c", &s->module_status, &c) || c != '\n') {
        free(t);
        return NULL;
    }

    printf(">> status_change_date: ");
    if (scanf("%d.%d.%d%c", &t->tm_mday, &t->tm_mon, &t->tm_year, &c) != 4 || c != '\n') {
        free(t);
        return NULL;
    }
    strftime(s->status_change_date, 11, "%d.%m.%Y", t);

    printf(">> status_change_time: ");
    if (scanf("%d:%d:%d%c", &t->tm_hour, &t->tm_min, &t->tm_sec, &c) != 4 || c != '\n') {
        free(t);
        return NULL;
    }
    strftime(s->status_change_time, 9, "%H:%M:%S", t);

    free(t);
    return s;
}

void output_status_event(status_event *s) {
    printf("%d %d %d %s %s\n", s->id,
                               s->module_id,
                               s->module_status,
                               s->status_change_date,
                               s->status_change_time);
}

void execute_query(int table_number, int operation_number) {
    FILE *db = NULL;
    if (table_number == 1) {
        db = connect(MASTER_MODULES_DB, "rb+");
        if (db == NULL) {
            printf("Error db connection\n");
            return;
        }
        module *m = malloc(sizeof(module));
        if (m == NULL) {
            printf("Error allocating memory\n");
            return;
        }
        if (operation_number == 1) {
            int id = input_selected_id();
            if (id != -1) {
                createidx(db, module_entity, sizeof(module), m, get_module_id);
            }
            m = select_module(db, m, id);
            if (m == NULL) {
                printf("Module not found\n");
            } else {
                if (id == -1) {
                    for (int i = 0; m[i].id != -1; i++) {
                        output_module(&m[i]);
                    }
                } else {
                    output_module(m);
                }
            }
        } else if (operation_number == 2) {
            module *data = malloc(sizeof(module));
            data = input_module(data);
            if (data == NULL) {
                printf("Invalid input\n");
            } else {
                int status = insert_module(db, m, data);
                if (status == 0) {
                    printf("Module already exists\n");
                } else {
                    printf("Module inserted\n");
                }
                free(data);
            }
        } else if (operation_number == 3) {
            int id = input_id("module");
            if (id == -1) {
                printf("Invalid input\n");
            } else {
                createidx(db, module_entity, sizeof(module), m, get_module_id);
                if (select_module(db, m, id) == NULL) {
                    printf("Module not found\n");
                } else {
                    module *data = malloc(sizeof(module));
                    data = input_module(data);
                    if (data == NULL) {
                        printf("Invalid input\n");
                    } else {
                        int status = update_module(db, m, data, id);
                        if (status == 0) {
                            printf("Module not found\n");
                        } else {
                            printf("Module updated\n");
                        }
                        free(data);
                    }
                }
            }
        } else if (operation_number == 4) {
        }
        free(m);
    } else if (table_number == 2) {
        db = connect(MASTER_LEVELS_DB, "rb+");
        if (db == NULL) {
            printf("Error db connection\n");
            return;
        }
        level *l = malloc(sizeof(level));
        if (l == NULL) {
            printf("Error allocating memory\n");
            return;
        }
        if (operation_number == 1) {
            int id = input_selected_id();
            if (id != -1) {
                createidx(db, level_entity, sizeof(level), l, get_level_id);
            }
            l = select_level(db, l, id);
            if (l == NULL) {
                printf("Level not found\n");
            } else {
                if (id == -1) {
                    for (int i = 0; l[i].id != -1; i++) {
                        output_level(&l[i]);
                    }
                } else {
                    output_level(l);
                }
            }
        } else if (operation_number == 2) {
            level *data = malloc(sizeof(level));
            data = input_level(data);
            if (data == NULL) {
                printf("Invalid input\n");
            } else {
                int status = insert_level(db, l, data);
                if (status == 0) {
                    printf("Level already exists\n");
                } else {
                    printf("Level inserted\n");
                }
                free(data);
            }
        } else if (operation_number == 3) {
            int id = input_id("level");
            if (id == -1) {
                printf("Invalid input\n");
            } else {
                createidx(db, level_entity, sizeof(level), l, get_level_id);
                if (select_level(db, l, id) == NULL) {
                    printf("Level not found\n");
                } else {
                    level *data = malloc(sizeof(level));
                    data = input_level(data);
                    if (data == NULL) {
                        printf("Invalid input\n");
                    } else {
                        int status = update_level(db, l, data, id);
                        if (status == 0) {
                            printf("Level not found\n");
                        } else {
                            printf("Level updated\n");
                        }
                        free(data);
                    }
                }
            }
        } else if (operation_number == 4) {
        }
        free(l);
    } else if (table_number == 3) {
        db = connect(MASTER_STATUS_EVENTS_DB, "rb+");
        if (db == NULL) {
            printf("Error db connection\n");
            return;
        }
        status_event *s = malloc(sizeof(status_event));
        if (s == NULL) {
            printf("Error allocating memory\n");
            return;
        }
        if (operation_number == 1) {
            int id = input_selected_id();
            if (id != -1) {
                createidx(db, status_event_entity, sizeof(status_event), s, get_status_event_id);
            }
            s = select_status_event(db, s, id);
            if (s == NULL) {
                printf("Status event not found\n");
            } else {
                if (id == -1) {
                    for (int i = 0; s[i].id != -1; i++) {
                        output_status_event(&s[i]);
                    }
                } else {
                    output_status_event(s);
                }
            }
        } else if (operation_number == 2) {
            status_event *data = malloc(sizeof(status_event));
            data = input_status_event(data);
            if (data == NULL) {
                printf("Invalid input\n");
            } else {
                int status = insert_status_event(db, s, data);
                if (status == 0) {
                    printf("Status event already exists\n");
                } else {
                    printf("Status event inserted\n");
                }
                free(data);
            }
        } else if (operation_number == 3) {
            int id = input_id("status event");
            if (id == -1) {
                printf("Invalid input\n");
            } else {
                createidx(db, status_event_entity, sizeof(status_event), s, get_status_event_id);
                if (select_status_event(db, s, id) == NULL) {
                    printf("Status event not found\n");
                } else {
                    status_event *data = malloc(sizeof(status_event));
                    data = input_status_event(data);
                    if (data == NULL) {
                        printf("Invalid input\n");
                    } else {
                        int status = update_status_event(db, s, data, id);
                        if (status == 0) {
                            printf("Status event not found\n");
                        } else {
                            printf("Status event updated\n");
                        }
                        free(data);
                    }
                }
            }
        } else if (operation_number == 4) {
        }
        free(s);
    }
    disconnect(db);
}
