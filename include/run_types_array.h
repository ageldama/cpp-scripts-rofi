#ifndef RUN_TYPES_ARRAY_H
#define RUN_TYPES_ARRAY_H

#include "run_types.h"
#include "m-array.h"

// => run_types_array_t
ARRAY_DEF(run_types_array, run_types_t)

#define M_OPL_run_types_array_t() ARRAY_OPLIST(run_types_array, M_BASIC_OPLIST)


#endif /* RUN_TYPES_ARRAY_H */

