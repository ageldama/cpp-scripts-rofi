#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <wordexp.h>

void expand_tilde(const char *inp, char *outp, const size_t outp_size) {
  wordexp_t we;

  if (wordexp(inp, &we, WRDE_NOCMD) == 0)
    {
      memset(outp, 0, outp_size);
      snprintf(outp, outp_size, we.we_wordv[0]);
      wordfree(&we);
    }
}
