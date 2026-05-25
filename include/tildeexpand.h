#ifndef TILDEEXPAND_H
#define TILDEEXPAND_H

#include "extern_.h"
#include "m-string.h"
#include "stdbool.h"

EXTERN_ void expand_tilde(
    const char* inp, char* outp, const size_t outp_size);

EXTERN_ void expand_tilde_string(const char* inp, string_t output);

EXTERN_ bool expand_tilde_string_fn(
    string_t* out, const string_t in, void* data);

#endif /* TILDEEXPAND_H */
