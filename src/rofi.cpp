#include "rofi.hpp"
#include "exec.hpp"

#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/wait.h>
#include <unistd.h>

namespace SR::rofi {

ssize_t pipe_write(const int fd, const char* s)
{
    return write(fd, s, strlen(s));
}

ssize_t pipe_write_and_sepchar(
    const int fd, const char* s, const char sep)
{
  ssize_t written = pipe_write(fd, s);
  std::array<char, 1> buf{ sep };
    written += write(fd, buf, 1);
    return written;
}

std::string read_all_fd(int fd)
{
    std::string result;
    constexpr size_t buflen = 4096;
    std::vector<char> buffer(buflen);
    ssize_t nread;

    while ((nread = read(fd, buffer.data(), buffer.size())) > 0) {
        result.append(buffer.data(), nread);
    }

    return result;
}

std::tuple<bool, int, std::string> run_rofi(
    const SR::string_vector& cmdv, const rofi_write_fn& write_fn)
{
  std::array<int, 2> p_to_c; // parent => child
  std::array<int, 2> c_to_p; // child => parent

  if (pipe(p_to_c.data()) == -1 || pipe(c_to_p.data()) == -1) {
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

        auto exec_errmsg = SR::exec::execvp(cmdv);

        fprintf(stderr, "%s\n", exec_errmsg.c_str());
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

void show_error(const std::string message)
{
    SR::string_vector cmdv = {
        "rofi",
        "-e",
        message,
    };

    SR::rofi::run_rofi(cmdv, [](const int fd) { (void)fd; });
}
}
