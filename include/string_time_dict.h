#ifndef STRING_TIME_DICT_H
#define STRING_TIME_DICT_H

#include "m-string.h"
#include "m-dict.h"
#include <time.h>

// => string_time_dict_t
DICT_DEF2(string_time_dict, string_t, M_STRING_OPLIST, time_t, M_BASIC_OPLIST)


#endif /* STRING_TIME_DICT_H */

