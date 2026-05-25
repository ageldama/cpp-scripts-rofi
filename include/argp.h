#ifndef ARGP_H
#define ARGP_H

#include "extern_.h"
#include <stdio.h>
#include <stdbool.h>
#include "m-string.h"
#include "string_list.h"

#define SCRIPT_ROFI_NO_DB_FLAG_FILE "~/.no-db-scripts-rofi"
#define SCRIPT_ROFI_DB_FLAG_FILE "~/.scripts-rofi.hist"
#define SCRIPT_ROFI_XTERM_COMMAND "x-terminal-emulator -e"
#define SCRIPT_ROFI_SCRIPT_DIRS "~/local/scripts:~/local/bin:~/.screenlayout:~/P/v3/bin"

EXTERN_ bool argp_v_print;
EXTERN_ bool argp_v_save;
EXTERN_ bool argp_v_execute;
EXTERN_ string_list_t argp_v_script_dirs;
EXTERN_ string_t argp_v_db_file;
EXTERN_ string_t argp_v_term_command;
EXTERN_ bool argp_v_dump_and_exit;
EXTERN_ string_t argp_v_exec_wrapper;
EXTERN_ string_list_t argp_v_file_regexes;
EXTERN_ bool argp_v_ignorecase;
EXTERN_ string_t argp_v_no_db_flag_file;

EXTERN_ void argp_init();
EXTERN_ void argp_cleanup();
EXTERN_ int argp_parse(const int argc, char* argv[]);
EXTERN_ void argp_print_usage(FILE* fp);

#endif /* ARGP_H */
