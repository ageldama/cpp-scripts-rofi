#include "tildeexpand.hpp"

#include <climits> // PATH_MAX
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <string>

#include <wordexp.h>

namespace SR::tilde {
void expand_buf(const char* inp, char* outp, const size_t outp_size)
{
    wordexp_t we;

    if (wordexp(inp, &we, WRDE_NOCMD) == 0) {
        memset(outp, 0, outp_size);
        strncpy(outp, we.we_wordv[0], outp_size);
        wordfree(&we);
    }
}

void expand(const char* inp, std::string& output)
{
    char* buf = static_cast<char*>(malloc(PATH_MAX));
    memset(buf, 0, PATH_MAX);
    expand_buf(inp, buf, PATH_MAX - 1);
    output.assign(buf, strnlen(buf, PATH_MAX));
    free(buf);
}
}
