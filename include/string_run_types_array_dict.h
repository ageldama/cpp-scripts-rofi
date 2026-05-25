#ifndef STRING_RUN_TYPES_ARRAY_DICT_H
#define STRING_RUN_TYPES_ARRAY_DICT_H

#include "run_types_array.h"

#include "m-dict.h"
#include "m-string.h"

// => string_run_types_array_dict_t
DICT_DEF2(string_run_types_array_dict, string_t, M_STRING_OPLIST,
    run_types_array_t, M_OPL_run_types_array_t())

#define M_OPL_string_run_types_array_dict_t()                        \
    DICT_OPLIST(string_run_types_array_dict, M_STRING_OPLIST,        \
        M_OPL_run_type_array_t())

#endif /* STRING_RUN_TYPES_ARRAY_DICT_H */
