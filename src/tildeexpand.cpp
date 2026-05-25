#include "tildeexpand.hpp"

#include <climits> // PATH_MAX
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <string>

#include <wordexp.h>

namespace SR::tilde {
void expand_buf(
    const char* inp, char* outp, const size_t outp_size)
{
    wordexp_t we;

    if (wordexp(inp, &we, WRDE_NOCMD) == 0) {
        memset(outp, 0, outp_size);
        snprintf(outp, outp_size, we.we_wordv[0]);
        wordfree(&we);
    }
}

void expand(const char* inp, std::string& output)
{
    const size_t maxlen = PATH_MAX + 1;
    char* buf = static_cast<char*>(malloc(maxlen));
    memset(buf, 0, maxlen);
    expand_buf(inp, buf, maxlen);
    output.assign(buf, strnlen(buf, maxlen));
    free(buf);
}
}
