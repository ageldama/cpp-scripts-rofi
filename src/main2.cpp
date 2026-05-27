#include "main.hpp"

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <optional>
#include <stdexcept>
#include <unistd.h>

#include "argp.hpp"
#include "db+rofi.hpp"
#include "db.hpp"
#include "exec.hpp"
#include "file_find.hpp"
#include "main.hpp"
#include "rofi.hpp"
#include "str.hpp"

namespace SR {
void main2(const std::span<char*>& args)
{
    using namespace std;
    using namespace SR;

    int rc = 0;

    argp::init();
    ::atexit(argp::cleanup);
    rc = argp::parse(args);
    (void)rc;

    db::init();
    ::atexit(db::cleanup);

    if (argp::db_load_allowed()) {
        try {
            db::load(argp::v_db_file.c_str());
        } catch (db::FileNotFoundException& exc) {
            cerr << "[IGNORE] load error (file not found) "
                 << exc.what() << endl;
        }
    }

    if (argp::v_dump_and_exit) {
        print_dump();
    }

    //
    auto files = sorted_file_list();

    auto opts = SR::rofi::rofi_common_opts {
        .prompt
        = "Select a script to run (Shift-Enter == toggle:terminal)",
        .ignorecase = SR::argp::v_ignorecase,
        .addopts = std::string(),
    };
    SR::db::db_run_alt_callbacks cbs;
    auto res = SR::rofi::select_list(opts, cbs, files);
    if (!res) {
        throw std::runtime_error("User cancelled (select_list)");
    }

    const auto cmd = res.value().stdout;
    const bool run_alt = res.value().alt;

    if (argp::db_save_allowed()) {
        db::upd_last_epoch(cmd);
        // db::set_run_alt(cmd, run_alt);
        const auto saved = db::save(argp::v_db_file.c_str());
        if (!saved) {
            cerr << "[IGNORE] save error: " + argp::v_db_file << endl;
        }
    }

    auto cmdv = string_vector {
        cmd,
    };

    if (!argp::v_exec_wrapper.empty()) {
        str::tokenize_cmd_and_prepend(cmdv, argp::v_exec_wrapper);
    }

    if (run_alt) {
        str::tokenize_cmd_and_prepend(cmdv, argp::v_term_command);
    }

    if (argp::v_print) {
        for (const auto& cmd : cmdv) {
            cout << cmd << " ";
        }
        cout << endl;
    }

    if (argp::v_execute) {
        const auto err_msg = exec::execvp(cmdv);
        rofi::show_error(err_msg);
        throw runtime_error("execvp fail: " + err_msg);
    }

    // all ok
}
}
