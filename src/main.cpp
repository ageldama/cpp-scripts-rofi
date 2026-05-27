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

auto ask_most_run_type(const std::string& cmd, bool alt)
    -> std::optional<SR::db::run_type_t>;

auto sorted_file_list() -> SR::string_vector;

void print_dump();

auto main(int argc, char** argv) -> int
{
    int rc = 0;

    auto args = std::span<char*>(argv, argc);

    argp::init();
    atexit(argp::cleanup);
    rc = argp::parse(args);
    (void)rc;

    db::init();
    atexit(db::cleanup);

    if (SR::argp::db_load_allowed()) {
        try {
            SR::db::load(SR::argp::v_db_file.c_str());
        } catch (SR::db::FileNotFoundException& exc) {
            std::cerr << "[IGNORE] load error (file not found) "
                      << exc.what() << std::endl;
        }
    }

    if (SR::argp::v_dump_and_exit) {
        print_dump();
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
    const auto final_run_type_opt
        = ask_most_run_type(cmd, res.value().alt);
    if (!final_run_type_opt) {
        exit(EXIT_FAILURE);
    }
    const auto final_run_type = final_run_type_opt.value();

    if (SR::argp::db_save_allowed()) {
        SR::db::upd_last_epoch(cmd);
        SR::db::incr_run_count(cmd, final_run_type);
        const auto saved = SR::db::save(SR::argp::v_db_file.c_str());
        if (!saved) {
            std::cerr << "[IGNORE] save error: " + SR::argp::v_db_file
                      << std::endl;
        }
    }

    auto cmdv = SR::string_vector {
        cmd,
    };

    if (!SR::argp::v_exec_wrapper.empty()) {
        SR::str::tokenize_cmd_and_prepend(
            cmdv, SR::argp::v_exec_wrapper);
    }

    if (final_run_type == SR::db::RUN_IN_TERM) {
        SR::str::tokenize_cmd_and_prepend(
            cmdv, SR::argp::v_term_command);
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
