#ifndef MODULES_DB_H
#define MODULES_DB_H

#include "shared.h"
#include "database.h"
#include "master_levels.h"
#include "master_modules.h"
#include "master_status_events.h"

int input_table();
int input_operation();

int input_length();
int input_id(const char *type);
int *input_ids(int ids[MAX], int length);
int input_selected_id();

level *input_level();
void output_level(level *l);

module *input_module();
void output_module(module *l);

status_event *input_status_event();
void output_status_event(status_event *s);

void execute_query(int table_number, int operation_number);
void execute_aggregation_query(int operation_number);

#endif
