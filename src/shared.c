#include <stdio.h>
#include <stdlib.h>

#include "shared.h"
#include "database.h"

void init_index(FILE *db, entity type, size_t sizeof_struct) {
    int j = 0; FILE *idx;
    struct index i;
    if (type == level_entity) {
        idx = connect(MASTER_LEVELS_IDX, "wb");
        level l;
        fseek(db, 0, SEEK_SET);
        while (fread(&l, sizeof_struct, 1, db) == 1) {
            i.id = l.id;
            i.index = j;
            fwrite(&i, sizeof(struct index), 1, idx);
            j++;
        }
        disconnect(idx);
    } else if (type == module_entity) {
        idx = connect(MASTER_MODULES_IDX, "wb");
        module m;
        fseek(db, 0, SEEK_SET);
        while (fread(&m, sizeof_struct, 1, db) == 1) {
            i.id = m.id;
            i.index = j;
            fwrite(&i, sizeof(struct index), 1, idx);
            j++;
        }
        disconnect(idx);
    } else if (type == status_event_entity) {
        idx = connect(MASTER_STATUS_EVENTS_IDX, "wb");
        status_event s;
        fseek(db, 0, SEEK_SET);
        while (fread(&s, sizeof_struct, 1, db) == 1) {
            i.id = s.id;
            i.index = j;
            fwrite(&i, sizeof(struct index), 1, idx);
            j++;
        }
        disconnect(idx);
    }
}

int find_index(entity type, int id) {
    FILE *idx;
    struct index i;
    if (type == level_entity) {
        idx = connect(MASTER_LEVELS_IDX, "rb+");
        fseek(idx, 0, SEEK_SET);
        while (fread(&i, sizeof(struct index), 1, idx) == 1) {
            if (i.id == id) {
                return i.index;
            }
        }
        disconnect(idx);
    } else if (type == module_entity) {
        idx = connect(MASTER_MODULES_IDX, "rb+");
        fseek(idx, 0, SEEK_SET);
        while (fread(&i, sizeof(struct index), 1, idx) == 1) {
            if (i.id == id) {
                return i.index;
            }
        }
        disconnect(idx);
    } else if (type == status_event_entity) {
        idx = connect(MASTER_STATUS_EVENTS_IDX, "rb+");
        fseek(idx, 0, SEEK_SET);
        while (fread(&i, sizeof(struct index), 1, idx) == 1) {
            if (i.id == id) {
                return i.index;
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
