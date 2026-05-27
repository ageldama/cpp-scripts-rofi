#ifndef ROFI_HPP
#define ROFI_HPP

#include "string_vector.hpp"
#include <functional>
#include <optional>
#include <string>
#include <tuple>

namespace SR::rofi {

using rofi_write_fn = std::function<void(const int fd)>;

ssize_t pipe_write(const int fd, const char* s);
ssize_t pipe_write_and_sepchar(
    const int fd, const char* s, const char sep);

 std::string read_all_fd(int fd);

 std::tuple<bool, int, std::string> run_rofi(
    const SR::string_vector& cmdv, const rofi_write_fn& write_fn);

 rofi_result {
    int exitcode;
    bool alt;
    std::string stdout;
};

 rofi_common_opts {
    std::string prompt;
    bool ignorecase;
    std::string addopts;
};

 std::optional<rofi_result> ask_yn(
    const rofi_common_opts& common_opts, const std::string& label_y,
    const std::string& label_n);

 std::optional<rofi_result> select_list(
    const rofi_common_opts& common_opts,
    const SR::string_vector& sel_list);

 void show_error(const std::string message);

}

#endif /* ROFI_HPP */
