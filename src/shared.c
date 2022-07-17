#include <stdio.h>
#include <stdlib.h>

#include "shared.h"
#include "database.h"

void init_index(FILE *db, entity type, size_t sizeof_struct) {
    int j = 0; FILE *idx;
    index i;
    if (type == level_entity) {
        idx = connect(MASTER_LEVELS_IDX, "wb");
        level l;
        fseek(db, 0, SEEK_SET);
        while (fread(&l, sizeof_struct, 1, db) == 1) {
            i.id = l.id;
            i.pos = j;
            fwrite(&i, sizeof(index), 1, idx);
            j++;
        }
        disconnect(idx);
    } else if (type == module_entity) {
        idx = connect(MASTER_MODULES_IDX, "wb");
        module m;
        fseek(db, 0, SEEK_SET);
        while (fread(&m, sizeof_struct, 1, db) == 1) {
            i.id = m.id;
            i.pos = j;
            fwrite(&i, sizeof(index), 1, idx);
            j++;
        }
        disconnect(idx);
    } else if (type == status_event_entity) {
        idx = connect(MASTER_STATUS_EVENTS_IDX, "wb");
        status_event s;
        fseek(db, 0, SEEK_SET);
        while (fread(&s, sizeof_struct, 1, db) == 1) {
            i.id = s.id;
            i.pos = j;
            fwrite(&i, sizeof(index), 1, idx);
            j++;
        }
        disconnect(idx);
    }
}

int find_index(entity type, int id) {
    FILE *idx;
    index i;
    if (type == level_entity) {
        idx = connect(MASTER_LEVELS_IDX, "rb+");
        fseek(idx, 0, SEEK_SET);
        while (fread(&i, sizeof(index), 1, idx) == 1) {
            if (i.id == id) {
                return i.pos;
            }
        }
        disconnect(idx);
    } else if (type == module_entity) {
        idx = connect(MASTER_MODULES_IDX, "rb+");
        fseek(idx, 0, SEEK_SET);
        while (fread(&i, sizeof(index), 1, idx) == 1) {
            if (i.id == id) {
                return i.pos;
            }
        }
        disconnect(idx);
    } else if (type == status_event_entity) {
        idx = connect(MASTER_STATUS_EVENTS_IDX, "rb+");
        fseek(idx, 0, SEEK_SET);
        while (fread(&i, sizeof(index), 1, idx) == 1) {
            if (i.id == id) {
                return i.pos;
            }
        }
        disconnect(idx);
    }
    return -1;
}

void *select(FILE *db, entity type, size_t sizeof_struct, int id) {
    void *instance = malloc(sizeof_struct);
    if (instance == NULL) {
        printf("Error allocating memory\n");
        exit(0);
    }
    if (type == level_entity) {
        instance = select_level(db, instance, sizeof_struct, id);
    } else if (type == module_entity) {
        instance = select_module(db, instance, sizeof_struct, id);
    } else if (type == status_event_entity) {
        instance = select_status_event(db, instance, sizeof_struct, id);
    }
    return instance;
}

int delete(FILE *db, entity type, size_t sizeof_struct, int id) {
    void *instance = malloc(sizeof_struct);
    if (instance == NULL) {
        printf("Error allocating memory\n");
        exit(0);
    }
    int result = 0;
    if (type == level_entity) {
        result = delete_level(db, instance, sizeof_struct, id);
    } else if (type == module_entity) {
        result = delete_module(db, instance, sizeof_struct, id);
    } else if (type == status_event_entity) {
        result = delete_status_event(db, instance, sizeof_struct, id);
    }
    free(instance);
    return result;
}

int insert(FILE *db, entity type, size_t sizeof_struct, void *entity) {
    void *instance = malloc(sizeof_struct);
    if (instance == NULL) {
        printf("Error allocating memory\n");
        exit(0);
    }
    int result = 0;
    if (type == level_entity) {
        result = insert_level(db, instance, sizeof_struct, entity);
    } else if (type == module_entity) {
        result = insert_module(db, instance, sizeof_struct, entity);
    } else if (type == status_event_entity) {
        result = insert_status_event(db, instance, sizeof_struct, entity);
    }
    free(instance);
    return result;
}

int update(FILE *db, entity type, size_t sizeof_struct, void *entity, int id) {
    void *instance = malloc(sizeof_struct);
    if (instance == NULL) {
        printf("Error allocating memory\n");
        exit(0);
    }
    int result = 0;
    if (type == level_entity) {
        result = update_level(db, instance, sizeof_struct, entity, id);
    } else if (type == module_entity) {
        result = update_module(db, instance, sizeof_struct, entity, id);
    } else if (type == status_event_entity) {
        result = update_status_event(db, instance, sizeof_struct, entity, id);
    }
    free(instance);
    return result;
}

module *select_active_modules(FILE *status_event_db, FILE *module_db) {
    status_event *s = malloc(sizeof(status_event));
    if (s == NULL) {
        printf("Error allocating memory\n");
        free(s);
        exit(0);
    }
    int i = 0;
    while (fread(&s[i], sizeof(status_event), 1, status_event_db)) {
        if (s[i].module_status == 1) {
            i++;
            s = realloc(s, sizeof(status_event) * (i + 1));
            if (s == NULL) {
                printf("Error allocating memory\n");
                free(s);
                exit(0);
            }
        }
    }
    s[i].id = -1;
    if (i < 1) {
        free(s);
        return NULL;
    }
    module *m = malloc(sizeof(module));
    if (m == NULL) {
        printf("Error allocating memory\n");
        free(s);
        free(m);
        exit(0);
    }
    int j = 0;
    while (fread(&m[j], sizeof(module), 1, module_db)) {
        for (int k = 0; k < i; k++) {
            if (m[j].id == s[k].module_id) {
                j++;
                m = realloc(m, sizeof(module) * (j + 1));
                if (m == NULL) {
                    printf("Error allocating memory\n");
                    free(s);
                    free(m);
                    exit(0);
                }
            }
        }
    }
    free(s);
    m[j].id = -1;
    if (j < 1) {
        free(m);
        return NULL;
    }
    return m;
}

module *select_modules_by_level_id(FILE *db, int level_id) {
    module *m = malloc(sizeof(module));
    if (m == NULL) {
        printf("Error allocating memory\n");
        free(m);
        exit(0);
    }
    int i = 0;
    while (fread(&m[i], sizeof(module), 1, db)) {
        if (m[i].level_id == level_id) {
            i++;
            m = realloc(m, sizeof(module) * (i + 1));
            if (m == NULL) {
                printf("Error allocating memory\n");
                free(m);
                exit(0);
            }
        }
    }
    m[i].id = -1;
    if (i < 1) {
        return NULL;
    }
    return m;
}

module *select_active_additional_modules(FILE *status_event_db, FILE *module_db) {
    status_event *s = malloc(sizeof(status_event));
    if (s == NULL) {
        printf("Error allocating memory\n");
        exit(0);
    }
    int i = 0;
    while (fread(&s[i], sizeof(status_event), 1, status_event_db)) {
        if (s[i].module_status == 1 && s[i].module_id != 0) {
            i++;
            s = realloc(s, sizeof(status_event) * (i + 1));
            if (s == NULL) {
                printf("Error allocating memory\n");
                free(s);
                exit(0);
            }
        }
    }
    s[i].id = -1;
    if (i < 1) {
        free(s);
        return NULL;
    }
    module *m = malloc(sizeof(module));
    if (m == NULL) {
        printf("Error allocating memory\n");
        free(s);
        free(m);
        exit(0);
    }
    int j = 0;
    while (fread(&m[j], sizeof(module), 1, module_db)) {
        for (int k = 0; k < i; k++) {
            if (m[j].id == s[k].module_id) {
                j++;
                m = realloc(m, sizeof(module) * (j + 1));
                if (m == NULL) {
                    printf("Error allocating memory\n");
                    free(s);
                    free(m);
                    exit(0);
                }
            }
        }
    }
    free(s);
    m[j].id = -1;
    if (j < 1) {
        free(m);
        return NULL;
    }
    return m;
}

int delete_modules_by_ids(FILE *db, int ids[MAX], int length) {
    module m;
    int found = 0;
    fseek(db, 0, SEEK_SET);
    while (fread(&m, sizeof(module), 1, db) == 1) {
        for (int i = 0; i < length; i++) {
            if (m.id == ids[i]) {
                m.deletion_flag = 1;
                fseek(db, -sizeof(module), SEEK_CUR);
                fwrite(&m, sizeof(module), 1, db);
                found = 1;
            }
        }
    }
    return found;
}

int set_level_protection_flag(FILE *db, int id) {
    level l;
    fseek(db, 0, SEEK_SET);
    while (fread(&l, sizeof(level), 1, db) == 1) {
        if (l.id == id) {
            l.protection_flag = 1;
            fseek(db, -sizeof(level), SEEK_CUR);
            fwrite(&l, sizeof(level), 1, db);
            return 1;
        }
    }
    return 0;
}

int set_module_protected_mode(FILE *module_db, FILE *status_event_db, int id) {
    module m;
    int found = 0;
    fseek(module_db, 0, SEEK_SET);
    while (fread(&m, sizeof(module), 1, module_db) == 1) {
        if (m.id == id) {
            found = 1;
            break;
        }
    }
    if (found) {
        status_event s;
        fseek(status_event_db, 0, SEEK_SET);
        while (fread(&s, sizeof(status_event), 1, status_event_db) == 1) {
            if (s.module_id == m.id) {
                s.module_status = 1;
                fseek(status_event_db, -sizeof(status_event), SEEK_CUR);
                fwrite(&s, sizeof(status_event), 1, status_event_db);
            }
        }
    }
    return found;
}

int set_main_module_protected_mode(FILE *db) {
    status_event s;
    int found = 0;
    fseek(db, 0, SEEK_SET);
    while (fread(&s, sizeof(status_event), 1, db) == 1) {
        if (s.module_id == 0) {
            s.module_status = 0;
            fseek(db, -sizeof(status_event), SEEK_CUR);
            fwrite(&s, sizeof(status_event), 1, db);
            s.module_status = 1;
            fseek(db, -sizeof(status_event), SEEK_CUR);
            fwrite(&s, sizeof(status_event), 1, db);
            s.module_status = 20;
            fseek(db, -sizeof(status_event), SEEK_CUR);
            fwrite(&s, sizeof(status_event), 1, db);
            found = 1;
            break;
        }
    }
    return found;
}

int move_main_module_with_protection_flag(FILE *module_db, FILE *level_db) {
    module m;
    int found = 0;
    fseek(module_db, 0, SEEK_SET);
    while (fread(&m, sizeof(module), 1, module_db) == 1) {
        if (m.id == 0) {
            m.level_id = 1;
            m.cell_id = 1;
            fseek(module_db, -sizeof(module), SEEK_CUR);
            fwrite(&m, sizeof(module), 1, module_db);
            found = 1;
        }
    }
    if (found) {
        set_level_protection_flag(level_db, 1);
    }
    return found;
}

int move_module_to_level_cell(FILE *db, int id, int level_id, int cell_id) {
    module m;
    fseek(db, 0, SEEK_SET);
    while (fread(&m, sizeof(module), 1, db) == 1) {
        if (m.id == id) {
            m.level_id = level_id;
            m.cell_id = cell_id;
            fseek(db, -sizeof(module), SEEK_CUR);
            fwrite(&m, sizeof(module), 1, db);
            return 1;
        }
    }
    return 0;
}

void turn_off_active_additional_modules(FILE *db) {
    status_event s;
    fseek(db, 0, SEEK_SET);
    while (fread(&s, sizeof(status_event), 1, db) == 1) {
        if (s.module_status == 1 && s.module_id != 0) {
            s.module_status = 0;
            fseek(db, -sizeof(status_event), SEEK_CUR);
            fwrite(&s, sizeof(status_event), 1, db);
        }
    }
}

void delete_active_additional_modules(FILE *db) {
    module m;
    fseek(db, 0, SEEK_SET);
    while (fread(&m, sizeof(module), 1, db) == 1) {
        if (m.id != 0) {
            m.deletion_flag = 1;
            fseek(db, -sizeof(module), SEEK_CUR);
            fwrite(&m, sizeof(module), 1, db);
        }
    }
}
