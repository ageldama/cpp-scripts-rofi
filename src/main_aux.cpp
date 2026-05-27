#include <algorithm>
#include <iostream>
#include <string>

#include "argp.hpp"
#include "db.hpp"
#include "file_find.hpp"
#include "string_vector.hpp"

namespace SR {
auto sorted_file_list() -> SR::string_vector
{
    auto files = string_vector {};

    if (SR::argp::v_file_regexes.empty()) {
        files = file_find::find_in_directories(
            SR::argp::v_script_dirs, file_find::only_file);
    } else {
        files
            = file_find::find_in_directories(SR::argp::v_script_dirs,
                file_find::make_only_file_and_regex_match(
                    SR::argp::v_file_regexes));
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
        cout << "\trun_alt =\t" << cmd_pair.second.run_alt << endl;
    }
}
}
