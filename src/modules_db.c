#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "modules_db.h"

int main(void) {
    system("clear");
    int table_number = input_table();
    int operation_number = input_operation();
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
        exit(0);
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
        exit(0);
    }
    return number;
}

int input_id(const char *type) {
    char c;
    int id;
    printf("Enter the %s id:\n"
           ">> ", type);
    if (!scanf("%d%c", &id, &c) || c != '\n') {
        exit(0);
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

module *input_module() {
    module *m = malloc(sizeof(module));
    if (m == NULL) {
        printf("Error allocating memory\n");
        exit(0);
    }
    char c;
    printf("Enter the attributes for the module:\n"
           ">> name: ");

    if (!scanf("%29s%c", m->name, &c) || c != '\n') {
        exit(0);
    }

    printf(">> level_id: ");
    if (!scanf("%d%c", &m->level_id, &c) || c != '\n') {
        exit(0);
    }

    printf(">> cell_id: ");
    if (!scanf("%d%c", &m->cell_id, &c) || c != '\n') {
        exit(0);
    }

    printf(">> deletion_flag: ");
    if (!scanf("%d%c", &m->deletion_flag, &c) || c != '\n') {
        exit(0);
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

level *input_level() {
    level *l = malloc(sizeof(level));
    if (l == NULL) {
        printf("Error allocating memory\n");
        exit(0);
    }
    char c;
    printf("Enter the attributes for the level:\n"
           ">> cell_amount: ");
    if (!scanf("%d%c", &l->cell_amount, &c) || c != '\n') {
        exit(0);
    }

    printf(">> protection_flag: ");
    if (!scanf("%d%c", &l->protection_flag, &c) || c != '\n') {
        exit(0);
    }
    return l;
}

void output_level(level *l) {
    printf("%d %d %d\n", l->id,
                         l->cell_amount,
                         l->protection_flag);
}

status_event *input_status_event() {
    status_event *s = malloc(sizeof(status_event));
    struct tm *t = malloc(sizeof(struct tm));
    if (s == NULL) {
        printf("Error allocating memory\n");
        exit(0);
    }
    char c;
    printf("Enter the attributes for the status event:\n"
           ">> module_id: ");
    if (!scanf("%d%c", &s->module_id, &c) || c != '\n') {
        exit(0);
    }

    printf(">> module_status: ");
    if (!scanf("%d%c", &s->module_status, &c) || c != '\n') {
        exit(0);
    }

    printf(">> status_change_date: ");
    if ((scanf("%d.%d.%d%c", &t->tm_mday, &t->tm_mon, &t->tm_year, &c) == 4) && c == '\n') {
        strftime(s->status_change_date, 11, "%d.%m.%Y", t);
    } else {
        exit(0);
    }

    printf(">> status_change_time: ");
    if ((scanf("%d:%d:%d%c", &t->tm_hour, &t->tm_min, &t->tm_sec, &c) == 4) && c == '\n') {
        strftime(s->status_change_time, 9, "%H:%M:%S", t);
    } else {
        exit(0);
    }

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
    if (table_number == 1) {
        FILE *db = connect(MASTER_MODULES_DB, "rb+");
        if (operation_number == 1) {
            module *m;
            int number = input_selected_id();
            if (number != -1) {
                m = select(db, module_entity, sizeof(module), number);
                if (m != NULL) {
                    output_module(m);
                }
                free(m);
            } else {
                m = select(db, module_entity, sizeof(module), -1);
                for (int i = 0; m[i].id != -1; i++) {
                    output_module(&m[i]);
                }
                free(m);
            }
        } else if (operation_number == 2) {
            if (insert(db, module_entity, sizeof(module), input_module())) {
                printf("Module inserted\n");
            } else {
                printf("Module already exist\n");
            }
        } else if (operation_number == 3) {
            if (update(db, module_entity, sizeof(module), input_module(), input_id("record"))) {
                printf("Module updated\n");
            } else {
                printf("Module not found\n");
            }
        } else if (operation_number == 4) {
            if (delete(db, module_entity, sizeof(module), input_id("record"))) {
                printf("Module deleted\n");
            } else {
                printf("Module not found\n");
            }
        }
        disconnect(db);
    } else if (table_number == 2) {
        FILE *db = connect(MASTER_LEVELS_DB, "rb+");
        if (operation_number == 1) {
            level *l;
            int number = input_selected_id();
            if (number != -1) {
                l = select(db, level_entity, sizeof(level), number);
                if (l != NULL) {
                    output_level(l);
                }
                free(l);
            } else {
                l = select(db, level_entity, sizeof(level), -1);
                for (int i = 0; l[i].id != -1; i++) {
                    output_level(&l[i]);
                }
                free(l);
            }
        } else if (operation_number == 2) {
            if (insert(db, level_entity, sizeof(level), input_level())) {
                printf("Level inserted\n");
            } else {
                printf("Level already exist\n");
            }
        } else if (operation_number == 3) {
            if (update(db, level_entity, sizeof(level), input_level(), input_id("record"))) {
                printf("Level updated\n");
            } else {
                printf("Level not found\n");
            }
        } else if (operation_number == 4) {
            if (delete(db, level_entity, sizeof(level), input_id("record"))) {
                printf("Level deleted\n");
            } else {
                printf("Level not found\n");
            }
        }
        disconnect(db);
    } else if (table_number == 3) {
        FILE *db = connect(MASTER_STATUS_EVENTS_DB, "rb+");
        if (operation_number == 1) {
            status_event *s;
            int number = input_selected_id();
            if (number != -1) {
                s = select(db, status_event_entity, sizeof(status_event), number);
                if (s != NULL) {
                    output_status_event(s);
                }
                free(s);
            } else {
                s = select(db, status_event_entity, sizeof(status_event), -1);
                for (int i = 0; s[i].id != -1; i++) {
                    output_status_event(&s[i]);
                }
                free(s);
            }
        } else if (operation_number == 2) {
            if (insert(db, status_event_entity, sizeof(status_event), input_status_event())) {
                printf("Status event inserted\n");
            } else {
                printf("Status event already exist\n");
            }
        } else if (operation_number == 3) {
            if (update(db, status_event_entity, sizeof(status_event),
                input_status_event(), input_id("record"))) {
                printf("Status event updated\n");
            } else {
                printf("Status event not found\n");
            }
        } else if (operation_number == 4) {
            if (delete(db, status_event_entity, sizeof(status_event), input_id("record"))) {
                printf("Status event deleted\n");
            } else {
                printf("Status event not found\n");
            }
        }
        disconnect(db);
    }
}
