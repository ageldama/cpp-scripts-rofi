#include "exec.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <unistd.h>

namespace SR::exec {
std::string execvp(const string_vector& cmdv)
{
    char** args = static_cast<char**>(
        malloc((cmdv.size() + 1) * sizeof(char*)));
    for (SR::string_vector::size_type i = 0; i < cmdv.size(); i++) {
        args[i] = const_cast<char*>(cmdv[i].c_str());
    }
    args[cmdv.size()] = nullptr;
    int rc = ::execvp(args[0], args);
    (void)rc;
    free(args); // never gonna be here, though.
    std::string errmsg(strerror(errno));
    return errmsg;
}
}
