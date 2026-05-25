#ifndef DB_H
#define DB_H

#include "extern_.h"
#include "run_types_array.h"
#include "string_run_types_array_dict.h"
#include "string_time_dict.h"

EXTERN_ string_time_dict_t db_v_cmd_last_epochs;
EXTERN_ string_run_types_array_dict_t db_v_cmd_run_types;

EXTERN_ void db_init();
EXTERN_ void db_cleanup();

EXTERN_ bool db_save(const char* filename);
EXTERN_ bool db_load(const char* filename);

#endif /* DB_H */
