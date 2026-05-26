#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <optional>
#include <stdexcept>
#include <unistd.h>

#include "argp.hpp"
#include "db.hpp"
#include "exec.hpp"
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
        throw std::invalid_argument("TODO print(db)");
    }

    //
    auto files = std::move(sorted_file_list());

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

    auto cmdv = SR::string_vector {
        cmd,
    };

    if (SR::argp::v_exec_wrapper != "") {
        // TODO vec.insert(vec.begin(), prefix.begin(), prefix.end());
    }

    if (final_run_type == SR::db::RUN_IN_TERM) {
        // TODO
    }

    if (SR::argp::v_print) {
        for (const auto& cmd : cmdv) {
            std::cout << cmd << " ";
        }
        std::cout << std::endl;
    }

    if (SR::argp::v_execute) {
        const auto err_msg = SR::exec::execvp(cmdv);
        SR::rofi::show_error(err_msg);
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
