#include "rofi.hpp"
#include "str.hpp"
#include "string_vector.hpp"

namespace SR::rofi {

using namespace SR;

  std::optional<rofi_result> select_list(const rofi_common_opts& common_opts,
                                       const SR::string_vector& sel_list)
{
    std::string ignorecase_opts = "";
  if (common_opts.ignorecase) ignorecase_opts = "-i";

    SR::string_vector cmdv = {
        "rofi",
        ignorecase_opts,
        common_opts.addopts,
        "-dmenu",
        "-p",
        common_opts.prompt,
        "-sep",
        R"(\0)",
        "-kb-accept-alt",
        R"()",
        "-kb-custom-1",
        R"(Shift+Return)",
    };


    auto res = SR::rofi::run_rofi(cmdv, [&sel_list](const int fd) {
      for (const auto &item :sel_list){
        SR::rofi::pipe_write_and_sepchar(fd, item.c_str(), '\0');
      }
    });

    bool run_ok = false;
    int run_exitcode = 0;
    std::string run_stdout;
    std::tie(run_ok, run_exitcode, run_stdout) = res;

    std::string run_stdout_trimmed = str::trim(run_stdout);

    if (!run_ok || run_exitcode != 0)
        return std::nullopt;

    return std::make_optional(rofi_result{.exitcode= run_exitcode,
                                          .alt = run_exitcode > 256,
                                          .stdout = run_stdout_trimmed,});
}

}
