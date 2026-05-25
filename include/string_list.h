#ifndef STRING_LIST_H
#define STRING_LIST_H

#include "m-algo.h"
#include "m-list.h"
#include "m-string.h"

// => string_list_t
LIST_DEF(string_list, string_t, M_STRING_OPLIST)

#define M_OPL_string_list_t() LIST_OPLIST(string_list, STRING_OPLIST)

ALGO_DEF(string_list, M_OPL_string_list_t())

#endif /* STRING_LIST_H */
