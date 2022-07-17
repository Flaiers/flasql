#ifndef MASTER_MODULES_H
#define MASTER_MODULES_H

#undef MASTER_MODULES_DB
#define MASTER_MODULES_DB "master_modules.db"

typedef struct module {
    int id;
    char name[30];
    int level_id;
    int cell_id;
    int deletion_flag;
} module;

module *select_module(FILE *db, module *m, size_t sizeof_struct, int id);
int delete_module(FILE *db, module *m, size_t sizeof_struct, int id);
int insert_module(FILE *db, module *m, size_t sizeof_struct, module *entity);
int update_module(FILE *db, module *m, size_t sizeof_struct, module *entity, int id);

#endif
