#include "tildeexpand.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <wordexp.h>

void expand_tilde(const char *inp, char *outp, const size_t outp_size)
{
  wordexp_t we;

  if (wordexp(inp, &we, WRDE_NOCMD) == 0)
    {
      memset(outp, 0, outp_size);
      snprintf(outp, outp_size, we.we_wordv[0]);
      wordfree(&we);
    }
}


void expand_tilde_string(const char *inp, string_t output)
{
  char *buf = malloc(PATH_MAX+1);
  memset(buf, 0, PATH_MAX+1);
  expand_tilde(inp, buf, PATH_MAX+1);
  string_set_str(output, buf);
  free(buf);
}


bool expand_tilde_string_fn(string_t *out, const string_t in, void *data)
{
  (void)data;
  expand_tilde_string(string_get_cstr(in), *out);
  return true;
}
