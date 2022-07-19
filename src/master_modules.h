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

void *get_module(void *e, int i);
int get_module_id(void *e);
int set_module_id(void *e, int id);

module *select_module(FILE *db, module *m, int id);
int insert_module(FILE *db, module *m, module *data);
int update_module(FILE *db, module *m, module *data, int id);
int delete_module(FILE *db, module *m, int id);

#endif
