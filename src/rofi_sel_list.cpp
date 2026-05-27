#include "rofi.hpp"
#include "str.hpp"
#include "string_vector.hpp"
#include <cstdio>
#include <string>

namespace SR::rofi {

using namespace SR;

auto select_list(const rofi_select_list_opts& opts,
    run_alt_callbacks& callbacks, const SR::string_vector& sel_list)
    -> std::optional<rofi_result>
{
    string_vector::size_type preselect = 0;

    std::string ignorecase_opts;
    if (opts.common_opts.ignorecase) {
        ignorecase_opts = "-i";
    }

    std::string markup_rows_opts;
    if (opts.use_markup) {
        markup_rows_opts = "-markup-rows";
    }

l_show_rofi:
    auto preselect_str = std::to_string(preselect);

    SR::string_vector cmdv = {
        "rofi",
        ignorecase_opts,
        opts.common_opts.addopts,
        "-dmenu",
        "-p",
        opts.common_opts.prompt,
        "-sep",
        R"(\0)",
        "-kb-accept-alt",
        R"()",
        "-kb-custom-1",
        R"(Shift+Return)",
        "-format",
        "i",
        markup_rows_opts,
        "-selected-row",
        preselect_str,
    };

    auto res = SR::rofi::run_rofi(
        cmdv, [&sel_list, &callbacks, &opts](const int fd) {
            for (const auto& item : sel_list) {
                std::string item_lbl = item;
                if (callbacks.is_run_alt(item)) {
                    item_lbl = item + opts.run_alt_tag;
                }
                SR::rofi::pipe_write_and_sepchar(
                    fd, item_lbl.c_str(), '\0');
            }
        });

    bool run_ok = false;
    int run_exitcode = 0;
    std::string run_stdout;
    std::tie(run_ok, run_exitcode, run_stdout) = res;

    std::string run_stdout_trimmed = str::trim(run_stdout);

    constexpr int magic_exitcode = 256;

    if (!run_ok
        || (run_exitcode > 0 && run_exitcode <= magic_exitcode)) {
        return std::nullopt;
    }

    const string_vector::size_type nth
        = std::stol(run_stdout_trimmed);
    const auto nth_cmd = sel_list.at(nth);
    preselect = nth;

    if (run_exitcode > magic_exitcode) {
        callbacks.toggle_run_alt(nth_cmd);
        goto l_show_rofi;
    }

    return std::make_optional(rofi_result {
        .exitcode = run_exitcode,
        .alt = callbacks.is_run_alt(nth_cmd),
        .stdout = nth_cmd,
    });
}

}
