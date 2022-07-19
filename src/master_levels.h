#ifndef MASTER_LEVELS_H
#define MASTER_LEVELS_H

#undef MASTER_LEVELS_DB
#define MASTER_LEVELS_DB "master_levels.db"

typedef struct level {
    int id;
    int cell_amount;
    int protection_flag;
} level;

void *get_level(void *e, int i);
int get_level_id(void *e);
int set_level_id(void *e, int id);

level *select_level(FILE *db, level *l, int id);
int insert_level(FILE *db, level *l, level *data);
int update_level(FILE *db, level *l, level *data, int id);
int delete_level(FILE *db, level *l, int id);

#endif
