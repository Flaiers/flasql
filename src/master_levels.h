#ifndef MASTER_LEVELS_H
#define MASTER_LEVELS_H

#undef MASTER_LEVELS_DB
#define MASTER_LEVELS_DB "master_levels.db"

typedef struct level {
    int id;
    int cell_amount;
    int protection_flag;
} level;

level *select_level(FILE *db, level *l, size_t sizeof_struct, int id);
int delete_level(FILE *db, level *l, size_t sizeof_struct, int id);
int insert_level(FILE *db, level *l, size_t sizeof_struct, level *entity);
int update_level(FILE *db, level *l, size_t sizeof_struct, level *entity, int id);

#endif
