#include "tildeexpand.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <climits> // PATH_MAX

#include <string>

#include <wordexp.h>

void expand_tilde_buf(const char* inp, char* outp, const size_t outp_size)
{
    wordexp_t we;

    if (wordexp(inp, &we, WRDE_NOCMD) == 0) {
        memset(outp, 0, outp_size);
        snprintf(outp, outp_size, we.we_wordv[0]);
        wordfree(&we);
    }
}

void expand_tilde(const char* inp, std::string& output)
{
  const size_t maxlen = PATH_MAX+1;
  char* buf = static_cast<char*>(malloc(maxlen));
  memset(buf, 0, maxlen);
  expand_tilde_buf(inp, buf, maxlen);
    output.assign(buf, strnlen(buf, maxlen));
    free(buf);
}
