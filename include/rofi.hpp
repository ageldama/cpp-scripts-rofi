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

extern std::optional<std::string> ask_yn(const std::string& prompt,
    const std::string& label_y, const std::string& label_n);

extern std::optional<std::string> select_list(
    const std::string& prompt, const SR::string_vector& sel_list);

}

#endif /* ROFI_HPP */
