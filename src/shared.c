#include <stdio.h>
#include <stdlib.h>

#include "shared.h"
#include "database.h"
#include "master_levels.h"
#include "master_modules.h"
#include "master_status_events.h"

int getidx(entity type, int id) {
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
    if (idx != NULL) {
        fseek(idx, 0, SEEK_SET);
        while (fread(&indx, sizeof(struct index), 1, idx) == 1) {
            if (indx.id == id) {
                i = indx.index;
                break;
            }
        }
    }
    disconnect(idx);
    return i;
}

void createidx(FILE *db, entity type, size_t sizeof_entity, void *e, int (*getid)(void *)) {
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
    if (idx != NULL) {
        fseek(db, 0, SEEK_SET);
        while (fread(e, sizeof_entity, 1, db) == 1) {
            indx.id = getid(e);
            indx.index = i;
            fwrite(&indx, sizeof(struct index), 1, idx);
            i++;
        }
    }
    disconnect(idx);
}

void *select(FILE *db, entity type, size_t sizeof_entity, void *e, int id, void *(*get)(void *, int), int (*getid)(void *), int (*setid)(void *, int)) {
    if (id == -1) {
        int i = 0;
        while (fread(get(e, i), sizeof_entity, 1, db) == 1) {
            i++;
            e = realloc(e, sizeof_entity * (i + 1));
            if (e == NULL) {
                return NULL;
            }
        }
        setid(get(e, i), id);
        return e;
    } else {
        int index = getidx(type, id);
        if (index != -1) {
            fseek(db, index * sizeof_entity, SEEK_SET);
            fread(e, sizeof_entity, 1, db);
            return e;
        } else {
            fseek(db, 0, SEEK_SET);
            while (fread(e, sizeof_entity, 1, db) == 1) {
                if (getid(e) == id) {
                    return e;
                }
            }
        }
        return NULL;
    }
}

int insert(FILE *db, size_t sizeof_entity, void *e, void *data, int (*getid)(void *), int (*setid)(void *, int)) {
    fseek(db, 0, SEEK_SET);
    fread(e, sizeof_entity, 1, db);

    fseek(db, 0, SEEK_END);
    setid(data, (ftell(db) / sizeof_entity) + getid(e));
    fwrite(data, sizeof_entity, 1, db);
    return 1;
}

int update(FILE *db, entity type, size_t sizeof_entity, void *e, void *data, int id, int (*getid)(void *), int (*setid)(void *, int)) {
    setid(data, id);
    int index = getidx(type, id);
    if (index != -1) {
        fseek(db, index * sizeof_entity, SEEK_SET);
        fwrite(data, sizeof_entity, 1, db);
        return 1;
    } else {
        fseek(db, 0, SEEK_SET);
        while (fread(e, sizeof_entity, 1, db) == 1) {
            if (getid(e) == id) {
                fseek(db, -sizeof_entity, SEEK_CUR);
                fwrite(data, sizeof_entity, 1, db);
                return 1;
            }
        }
    }
    return 0;
}

int delete(FILE *db, entity type, size_t sizeof_entity, void *e, int id, void *(*get)(void *, int), int (*getid)(void *), int (*setid)(void *, int)) {
    e = select(db, type, sizeof_entity, e, -1, get, getid, setid);
    if (e == NULL) {
        return 0;
    }

    FILE *rdb = NULL;
    if (type == level_entity) {
        rdb = connect(MASTER_LEVELS_DB, "wb");
    } else if (type == module_entity) {
        rdb = connect(MASTER_MODULES_DB, "wb");
    } else if (type == status_event_entity) {
        rdb = connect(MASTER_STATUS_EVENTS_DB, "wb");
    }
    if (rdb == NULL) {
        return 0;
    }

    int status = 0;
    fseek(rdb, 0, SEEK_SET);
    for (int i = 0; getid(get(e, i)) != -1; i++) {
        if (getid(get(e, i)) == id) {
            status = 1;
        } else {
            fwrite(get(e, i), sizeof_entity, 1, rdb);
        }
    }
    disconnect(rdb);
    return status;
}
