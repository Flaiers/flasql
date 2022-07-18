#ifndef MODULES_DB_H
#define MODULES_DB_H

#include "shared.h"
#include "database.h"
#include "master_levels.h"
#include "master_modules.h"
#include "master_status_events.h"

int input_table();
int input_operation();

int input_id(const char *type);
int input_selected_id();

level *input_level(level *l);
void output_level(level *l);

module *input_module(module *m);
void output_module(module *m);

status_event *input_status_event(status_event *s);
void output_status_event(status_event *s);

void execute_query(int table_number, int operation_number);

#endif
