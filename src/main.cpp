#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>

#include "argp.hpp"
#include "db.hpp"
#include "file_find.hpp"
#include "rofi.hpp"
#include "str.hpp"

#include <iostream>

using namespace std;
using namespace SR;

int main(int argc, char* argv[])
{
    int rc = 0;

    argp::init();
    atexit(argp::cleanup);
    rc = argp::parse(argc, argv);
    (void)rc;

    db::init();
    atexit(db::cleanup);

    // FIXME
    if (SR::argp::v_db_file != "")
        SR::db::load(SR::argp::v_db_file.c_str());

    if (SR::argp::v_dump_and_exit) {
        // TODO print(db)
        exit(EXIT_SUCCESS);
    }

    //
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

    auto opts = SR::rofi::rofi_common_opts {
        .prompt
        = "Select a script to run (Shift-Enter == run-in-terminal)",
        .ignorecase = SR::argp::v_ignorecase,
    };
    auto res = SR::rofi::select_list(opts, files);

    if (!res) {
        exit(EXIT_FAILURE);
    }

    printf("exit:%d // alt:%d // cmd:[%s]\n", res.value().exitcode,
        res.value().alt, res.value().stdout.c_str());

    const auto cmd = res.value().stdout;
    SR::db::upd_last_epoch(cmd);
    SR::db::incr_run_count(cmd,
        res.value().alt ? SR::db::RUN_IN_TERM : SR::db::RUN_NORMAL);

    // FIXME
    if (SR::argp::v_db_file != "" && SR::argp::v_save)
        SR::db::save(SR::argp::v_db_file.c_str());

    // TODO print
    // TODO exec
    // TODO exec-wrapper
    // TODO exec-in-term
    // TODO SR::rofi::ask_yn("Y/N???", true, "", "YYYYYY", "NNNNN");

    exit(EXIT_SUCCESS);
}
