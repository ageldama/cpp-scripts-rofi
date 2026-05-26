#ifndef ROFI_HPP
#define ROFI_HPP

#include "string_vector.hpp"
#include <functional>
#include <optional>
#include <string>
#include <tuple>

namespace SR::rofi {

using rofi_write_fn = std::function<void(const int fd)>;

extern int pipe_write(const int fd, const char* s);
extern int pipe_write_and_sepchar(
    const int fd, const char* s, const char sep);

extern std::string read_all_fd(int fd);

extern std::tuple<bool, int, std::string> run_rofi(
    const SR::string_vector& cmdv, rofi_write_fn write_fn);

struct rofi_result {
    int exitcode;
    bool alt;
    std::string stdout;
};

struct rofi_common_opts {
    std::string prompt;
    bool ignorecase;
    std::string addopts;
};

extern std::optional<rofi_result> ask_yn(
    rofi_common_opts& common_opts, const std::string& label_y,
    const std::string& label_n);

extern std::optional<rofi_result> select_list(
    rofi_common_opts& common_opts, const SR::string_vector& sel_list);

}

#endif /* ROFI_HPP */
