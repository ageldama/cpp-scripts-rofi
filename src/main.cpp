#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <optional>
#include <unistd.h>

#include "argp.hpp"
#include "db.hpp"
#include "file_find.hpp"
#include "rofi.hpp"
#include "str.hpp"

using namespace std;
using namespace SR;

std::optional<SR::db::run_type_t> ask_most_run_type(
    const std::string& cmd, const bool alt);

SR::string_vector sorted_file_list();

int main(int argc, char* argv[])
{
    int rc = 0;

    argp::init();
    atexit(argp::cleanup);
    rc = argp::parse(argc, argv);
    (void)rc;

    db::init();
    atexit(db::cleanup);

    if (SR::argp::db_load_allowed())
        SR::db::load(SR::argp::v_db_file.c_str());

    if (SR::argp::v_dump_and_exit) {
        // TODO print(db)
        exit(EXIT_SUCCESS);
    }

    //
    auto files = sorted_file_list();

    auto opts = SR::rofi::rofi_common_opts {
        .prompt
        = "Select a script to run (Shift-Enter == run-in-terminal)",
        .ignorecase = SR::argp::v_ignorecase,
    };
    auto res = SR::rofi::select_list(opts, files);

    if (!res) {
        exit(EXIT_FAILURE);
    }

    const auto cmd = res.value().stdout;
    const auto final_run_type_
        = ask_most_run_type(cmd, res.value().alt);
    if (!final_run_type_)
        exit(EXIT_FAILURE);
    const auto final_run_type = final_run_type_.value();

    if (SR::argp::db_save_allowed()) {
          SR::db::upd_last_epoch(cmd);
          SR::db::incr_run_count(cmd, final_run_type);
          SR::db::save(SR::argp::v_db_file.c_str());
    }

    // TODO print
    // TODO exec
    // TODO exec-wrapper
    // TODO exec-in-term

    exit(EXIT_SUCCESS);
}

std::optional<SR::db::run_type_t> ask_most_run_type(
    const std::string& cmd, const bool alt)
{
    const auto most_run_type
        = SR::db::get_most_run_type(cmd, SR::db::RUN_NORMAL);

    const auto selected_run_type
        = alt ? SR::db::RUN_IN_TERM : SR::db::RUN_NORMAL;

    SR::db::run_type_t final_run_type = SR::db::RUN_NORMAL;

    if (selected_run_type != most_run_type) {
        auto opts_yn = SR::rofi::rofi_common_opts {
            .prompt
            = "Different run type from usual, Correct it usually?",
            .ignorecase = SR::argp::v_ignorecase,
        };
        const std::string run_normal("No Terminal");
        const std::string run_in_term("In terminal");
        const auto lbl_y = most_run_type == SR::db::RUN_NORMAL
            ? run_normal
            : run_in_term;
        const auto lbl_n = most_run_type == SR::db::RUN_NORMAL
            ? run_in_term
            : run_normal;
        auto res_yn = SR::rofi::ask_yn(opts_yn, lbl_y, lbl_n);
        if (res_yn) {
            if (res_yn.value().stdout == "0") {
                final_run_type = most_run_type;
            } else {
                final_run_type = selected_run_type;
            }
        } else {
            return std::nullopt;
        }
    }

    return std::make_optional(final_run_type);
}

SR::string_vector sorted_file_list()
{
    auto files = string_vector {};

    if (SR::argp::v_file_regexes.size() > 0) {
        files
            = file_find::find_in_directories(SR::argp::v_script_dirs,
                file_find::make_only_file_and_regex_match(
                    SR::argp::v_file_regexes));
    } else {
        files = file_find::find_in_directories(
            SR::argp::v_script_dirs, file_find::only_file);
    }

    std::sort(files.begin(), files.end(),
        [](const std::string& a, const std::string& b) {
            const auto a_epoch = SR::db::get_last_epoch(a);
            const auto b_epoch = SR::db::get_last_epoch(b);
            return a_epoch > b_epoch;
        });

    return files;
}
