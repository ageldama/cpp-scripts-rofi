#include "rofi.hpp"
#include "str.hpp"

namespace SR::rofi {

using namespace SR;

std::optional<rofi_result> ask_yn(const rofi_common_opts& common_opts,
    const std::string& label_y, const std::string& label_n)
{
  std::string ignorecase_opts;
    if (common_opts.ignorecase)
        ignorecase_opts = "-i";

    SR::string_vector cmdv = {
        "rofi",
        ignorecase_opts,
        common_opts.addopts,
        //"-theme-str",
        //"window {width: 200px; height: 150px;}",
        "-dmenu",
        "-p",
        common_opts.prompt,
        "-sep",
        R"(\0)",
        //"-eh",
        //"2",
        //"-markup-rows",
        "-format",
        "i",
    };

    // auto lbl_y = std::string("<span size='x-large'
    // weight='heavy'>")
    //     + label_y + std::string("</span>");
    // auto lbl_n = std::string("<span size='x-large'
    // weight='heavy'>")
    //     + label_n + std::string("</span>");

    auto res = SR::rofi::run_rofi(
        cmdv, [&label_y, &label_n](const int fd) {
            SR::rofi::pipe_write_and_sepchar(
                fd, label_y.c_str(), '\0');
            SR::rofi::pipe_write_and_sepchar(
                fd, label_n.c_str(), '\0');
        });

    bool run_ok = false;
    int run_exitcode = 0;
    std::string run_stdout;
    std::tie(run_ok, run_exitcode, run_stdout) = res;

    std::string run_stdout_trimmed = str::trim(run_stdout);

    if (!run_ok || run_exitcode != 0)
        return std::nullopt;

    return std::make_optional(rofi_result {
        .exitcode = run_exitcode,
        .alt = false,
        .stdout = run_stdout_trimmed,
    });
}

}
