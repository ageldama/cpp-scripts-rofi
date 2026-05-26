#include <algorithm>
#include <iostream>
#include <optional>
#include <string>

#include "argp.hpp"
#include "db.hpp"
#include "file_find.hpp"
#include "rofi.hpp"
#include "string_vector.hpp"

using namespace SR;

std::optional<SR::db::run_type_t> ask_most_run_type(
    const std::string& cmd, const bool alt)
{
    const auto most_run_type
        = SR::db::get_most_run_type(cmd, SR::db::RUN_UNKNOWN);

    const auto selected_run_type
        = alt ? SR::db::RUN_IN_TERM : SR::db::RUN_NORMAL;

    SR::db::run_type_t final_run_type = selected_run_type;

    if (most_run_type != SR::db::RUN_UNKNOWN
        && selected_run_type != most_run_type) {
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

void print_dump()
{
    using namespace std;
    using namespace SR;

    cout << "--- ARGS ---" << endl;
    cout << "* print? = \t" << argp::v_print << endl;
    cout << "* save? = \t" << argp::v_save << endl;
    cout << "* execute? = \t" << argp::v_execute << endl;

    cout << "* script_dirs = \t";
    for (const auto& script_dir : argp::v_script_dirs) {
        cout << script_dir << " ";
    }
    cout << endl;

    cout << "* file_regexes = \t";
    for (const auto& file_regex : argp::v_file_regexes) {
        cout << file_regex << " ";
    }
    cout << endl;

    cout << "* db_file = \t" << argp::v_db_file << endl;
    cout << "* term_command = \t" << argp::v_term_command << endl;
    cout << "* exec_wrapper = \t" << argp::v_exec_wrapper << endl;
    cout << "* dump_and_exit = \t" << argp::v_dump_and_exit << endl;
    cout << "* ignorecase = \t" << argp::v_ignorecase << endl;
    cout << "* no_db_flag_file = \t" << argp::v_no_db_flag_file
         << endl;

    cout << endl;

    cout << "--- DB ---" << endl;
    auto db_tot = db::v_db.size();
    cout << "* tot = \t" << db_tot << endl;
    size_t count = 0;
    for (const auto& cmd_pair : db::v_db) {
        count++;
        cout << count << "/" << db_tot << "\t" << cmd_pair.first
             << endl;
        cout << "\tlast =\t" << cmd_pair.second.last_epoch << endl;
        cout << "\trun_type_counts# =\t"
             << cmd_pair.second.run_type_counts.size() << endl;
        size_t run_type_idx = 0;
        for (const auto& run_type_count :
            cmd_pair.second.run_type_counts) {
            cout << "\trun-type:" << run_type_idx << " =\t"
                 << run_type_count << endl;
            run_type_idx++;
        }
    }
}
