#ifndef TILDEEXPAND_H
#define TILDEEXPAND_H

#include <string>

extern "C" {
void expand_tilde_buf(
    const char* inp, char* outp, const size_t outp_size);

void expand_tilde(const char* inp, std::string& output);
}

#endif /* TILDEEXPAND_H */
