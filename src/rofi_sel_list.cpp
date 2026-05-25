#include "rofi.hpp"
#include "str.hpp"
#include "string_vector.hpp"

namespace SR::rofi {

using namespace SR;

  // TODO
std::optional<std::string> select_list(const std::string& prompt,
                                       const SR::string_vector& sel_list)
{
    SR::string_vector cmdv = {
        "rofi",
        "-theme-str",
        "window {width: 200px; height: 150px;}",
        "-dmenu",
        "-p",
        prompt,
        "-sep",
        R"(\0)",
        "-eh",
        "2",
        "-markup-rows",
        "-format",
        "i",
    };

    auto lbl_y = std::string("<span size='x-large' weight='heavy'>")
        + label_y + std::string("</span>");
    auto lbl_n = std::string("<span size='x-large' weight='heavy'>")
        + label_n + std::string("</span>");

    auto res = SR::rofi::run_rofi(cmdv, [lbl_y, lbl_n](const int fd) {
        SR::rofi::pipe_write_and_sepchar(fd, lbl_y.c_str(), '\0');
        SR::rofi::pipe_write_and_sepchar(fd, lbl_n.c_str(), '\0');
    });

    bool run_ok = false;
    int run_exitcode = 0;
    std::string run_stdout;
    std::tie(run_ok, run_exitcode, run_stdout) = res;

    std::string run_stdout_trimmed = str::trim(run_stdout);

    if (!run_ok || run_exitcode != 0)
        return std::nullopt;

    return std::make_optional(run_stdout_trimmed);
}

}
