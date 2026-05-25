#include "rofi.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/wait.h>
#include <unistd.h>

namespace SR::rofi {

int pipe_write(const int fd, const char* s)
{
    return write(fd, s, strlen(s));
}

std::string read_all_fd(int fd)
{
    std::string result;
    std::vector<char> buffer(4096);
    ssize_t bytesRead;

    while ((bytesRead = read(fd, buffer.data(), buffer.size())) > 0) {
        result.append(buffer.data(), bytesRead);
    }

    return result;
}

std::tuple<bool, int, std::string> run_rofi(
    const SR::string_vector& cmdv, rofi_write_fn write_fn)
{
    int p_to_c[2]; // parent => child
    int c_to_p[2]; // child => parent

    if (pipe(p_to_c) == -1 || pipe(c_to_p) == -1) {
        return { false, 0, "" };
    }

    pid_t pid = fork();

    if (pid == 0) {
        // CHILD

        close(p_to_c[1]); // where parent writes.
        close(c_to_p[0]); // where parent reads.

        dup2(p_to_c[0], STDIN_FILENO);
        close(p_to_c[0]);

        dup2(c_to_p[1], STDOUT_FILENO);
        close(c_to_p[1]);

        char** args = static_cast<char**>(
            malloc((cmdv.size() + 1) * sizeof(char*)));
        for (SR::string_vector::size_type i = 0; i < cmdv.size();
             i++) {
            args[i] = const_cast<char*>(cmdv[i].c_str());
        }
        args[cmdv.size()] = nullptr;
        int rc = execvp(args[0], args);
        free(args); // never gonna be here, though.

        // if (rc<0)
        perror("execvp failed");
        (void)rc;
        exit(EXIT_FAILURE);
    }

    // Parent
    close(p_to_c[0]); // where child reads.
    close(c_to_p[1]); // where child writes.

    write_fn(p_to_c[1]);
    close(p_to_c[1]);

    int wstatus = 0;
    pid_t result = waitpid(pid, &wstatus, 0);
    (void)result;
    // printf("exit-code: %d\n", wstatus);

    std::string read = read_all_fd(c_to_p[0]);
    close(c_to_p[0]);
    // printf("READ: %s\n", read.c_str());

    return { true, wstatus, read };
}
}
