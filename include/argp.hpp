#ifndef ARGP_HPP
#define ARGP_HPP

#include "string_vector.hpp"
#include <cstdio>

#define SCRIPT_ROFI_NO_DB_FLAG_FILE "~/.no-db-scripts-rofi"
#define SCRIPT_ROFI_DB_FLAG_FILE "~/.scripts-rofi.hist"
#define SCRIPT_ROFI_XTERM_COMMAND "x-terminal-emulator -e"
#define SCRIPT_ROFI_SCRIPT_DIRS                                      \
    "~/local/scripts:~/local/bin:~/.screenlayout:~/P/v3/bin"

extern bool argp_v_print;
extern bool argp_v_save;
extern bool argp_v_execute;
extern string_vector argp_v_script_dirs;
extern std::string argp_v_db_file;
extern std::string argp_v_term_command;
extern bool argp_v_dump_and_exit;
extern std::string argp_v_exec_wrapper;
extern string_vector argp_v_file_regexes;
extern bool argp_v_ignorecase;
extern std::string argp_v_no_db_flag_file;

extern "C" {
void argp_init();
void argp_cleanup();
int argp_parse(const int argc, char* argv[]);
void argp_print_usage(FILE* fp);

void argp_set_file_regexes(const char* arg);
void argp_set_script_dirs(const char* arg);
}

#endif /* ARGP_HPP */
