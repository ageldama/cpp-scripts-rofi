#ifndef ROFI_HPP
#define ROFI_HPP

#include "string_vector.hpp"
#include <functional>
#include <optional>
#include <string>
#include <tuple>

namespace SR::rofi {

using rofi_write_fn = std::function<void(const int fd)>;

auto pipe_write(int fd, const char* s) -> ssize_t;
auto pipe_write_and_sepchar(int fd, const char* s, char sep)
    -> ssize_t;

auto read_all_fd(int fd) -> std::string;

auto run_rofi(
    const SR::string_vector& cmdv, const rofi_write_fn& write_fn)
    -> std::tuple<bool, int, std::string>;

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

auto ask_yn(const rofi_common_opts& common_opts,
    const std::string& label_y, const std::string& label_n)
    -> std::optional<rofi_result>;

void show_error(const std::string& message);

struct rofi_select_list_opts {
    rofi_common_opts common_opts;
    bool use_markup;
    std::string run_alt_tag;
};

class run_alt_callbacks {
public:
    virtual auto is_run_alt(const std::string& cmd) -> bool = 0;
    virtual auto toggle_run_alt(const std::string& cmd) -> bool = 0;
    virtual ~run_alt_callbacks() = default;
};

auto select_list(const rofi_select_list_opts& common_opts,
    run_alt_callbacks& callbacks, const SR::string_vector& sel_list)
    -> std::optional<rofi_result>;

}

#endif /* ROFI_HPP */
