#ifndef TILDEEXPAND_H
#define TILDEEXPAND_H

#include "extern_.h"
#include "m-string.h"

EXTERN_ void expand_tilde(const char *inp, char *outp, const size_t outp_size);

EXTERN_ void expand_tilde_string(const char *inp, string_t output);

#endif /* TILDEEXPAND_H */
