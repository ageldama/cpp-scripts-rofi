#ifndef TILDEEXPAND_H
#define TILDEEXPAND_H

#include <string>

namespace SR::tilde {

extern void expand_buf(
    const char* inp, char* outp, const size_t outp_size);

extern void expand(const char* inp, std::string& output);

}

#endif /* TILDEEXPAND_H */
