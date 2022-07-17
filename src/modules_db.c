#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "modules_db.h"

int main(void) {
    int operation_number = input_operation();
    if (operation_number > 0 && operation_number < 5) {
        execute_query(input_table(), operation_number);
    } else {
        execute_aggregation_query(operation_number);
    }
    return 0;
}

int input_operation() {
    char c;
    int number;
    system("clear");
    printf("Please choose one operation:\n"
           "    1. SELECT\n"
           "    2. INSERT\n"
           "    3. UPDATE\n"
           "    4. DELETE\n"
           "    5. Получить все включенные модули\n"
           "    6. Получить все модули на первом уровне памяти\n"
           "    7. Получить все активные дополнительные модули (статус последнего модуля равен 1)\n"
           "    8. Удаление модулей по идентификаторам\n"
           "    9. Установить защищенный режим для модуля по идентификатору\n"
           "   10. Перемещение модуля по идентификатору в указанный уровень памяти и ячейку\n"
           "   11. Установить флаг защиты указанного уровня памяти\n"
           "   12. Выключить все его включенные дополнительные модули (перевести их статус в 0)\n"
           "   13. Удалить записи про них (простановкой соответствующего флага в 1)\n"
           "   14. Главный модуль (c id 0) перевести в защищенный режим (последовательным переводом"
           " в статус 0, затем 1, затем - 20)\n"
           "   15. Переместить его в первую ячейку первого уровня памяти с выставлением этому уровню"
           " флага защищенности 1\n"
           "   16. Сгенерировать индексные файлы\n"
           ">> ");
    if (!scanf("%d%c", &number, &c) || number < 1 || number > 16 || c != '\n') {
        exit(0);
    }
    return number;
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

int input_length() {
    char c;
    int length;
    printf("Enter the length of ids array:\n"
           ">> ");
    if (!scanf("%d%c", &length, &c) || c != '\n') {
        exit(0);
    }
    return length;
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

int *input_ids(int ids[MAX], int length) {
    char c;
    for (int i = 0; i < length; i++) {
        printf("Enter the record id:\n"
               ">> ");
        if (!scanf("%d%c", &ids[i], &c) || c != '\n') {
            exit(0);
        }
    }
    return ids;
}

int input_selected_id() {
    char c;
    int num = 0;
    printf("Enter the selected id or -1 to output all of them:\n"
           ">> ");
    if (!scanf("%d%c", &num, &c) || c != '\n') {
        return -1;
    }
    return num;
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

void execute_aggregation_query(int operation_number) {
    FILE *level_db = connect(MASTER_LEVELS_DB, "rb+");
    FILE *module_db = connect(MASTER_MODULES_DB, "rb+");
    FILE *status_event_db = connect(MASTER_STATUS_EVENTS_DB, "rb+");
    if (operation_number == 5) {
        module *modules = select_active_modules(status_event_db, module_db);
        if (modules != NULL) {
            for (int i = 0; modules[i].id != -1; i++) {
                output_module(&modules[i]);
            }
        }
        free(modules);
    } else if (operation_number == 6) {
        module *modules = select_modules_by_level_id(module_db, 1);
        if (modules != NULL) {
            for (int i = 0; modules[i].id != -1; i++) {
                output_module(&modules[i]);
            }
        }
        free(modules);
    } else if (operation_number == 7) {
        module *modules = select_active_additional_modules(status_event_db, module_db);
        if (modules != NULL) {
            for (int i = 0; modules[i].id != -1; i++) {
                output_module(&modules[i]);
            }
        }
        free(modules);
    } else if (operation_number == 8) {
        int length = input_length(), ids[MAX];
        if (delete_modules_by_ids(module_db, input_ids(ids, length), length)) {
            printf("Modules deleted\n");
        } else {
            printf("Modules not found\n");
        }
    } else if (operation_number == 9) {
        if (set_module_protected_mode(module_db, status_event_db, input_id("record"))) {
            printf("Module protected mode seted\n");
        } else {
            printf("Module not found\n");
        }
    } else if (operation_number == 10) {
        int id = input_id("record"), level_id = input_id("level"), cell_id = input_id("cell");
        if (move_module_to_level_cell(module_db, id, level_id, cell_id)) {
            printf("Module moved\n");
        } else {
            printf("Module not found\n");
        }
    } else if (operation_number == 11) {
        if (set_level_protection_flag(level_db, input_id("record"))) {
            printf("Level protection flag seted\n");
        } else {
            printf("Level not found\n");
        }
    } else if (operation_number == 12) {
        turn_off_active_additional_modules(status_event_db);
        printf("Additional modules turned off\n");
    } else if (operation_number == 13) {
        delete_active_additional_modules(module_db);
        printf("Additional modules deleted\n");
    } else if (operation_number == 14) {
        if (set_main_module_protected_mode(status_event_db)) {
            printf("Main module protected mode seted\n");
        } else {
            printf("Main module not found\n");
        }
    } else if (operation_number == 15) {
        if (move_main_module_with_protection_flag(module_db, level_db)) {
            printf("Main module moved\n");
        } else {
            printf("Main module not found\n");
        }
    } else if (operation_number == 16) {
        init_index(level_db, level_entity, sizeof(level));
        init_index(module_db, module_entity, sizeof(module));
        init_index(status_event_db, status_event_entity, sizeof(status_event));
    }
    disconnect(level_db);
    disconnect(module_db);
    disconnect(status_event_db);
}
