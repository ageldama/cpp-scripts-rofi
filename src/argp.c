#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#include "argp.h"
#include "tildeexpand.h"


bool argp_v_print = false;
bool argp_v_save = false;
bool argp_v_execute = false;
string_list_t argp_v_script_dirs;
string_t argp_v_db_file;
string_t argp_v_term_command;
bool argp_v_dump_and_exit = false;
string_t argp_v_exec_wrapper;
string_list_t argp_v_file_regexes;
bool argp_v_ignorecase = false;
string_t argp_v_no_db_flag_file;


void argp_init()
{
  string_init(argp_v_db_file);
  string_init(argp_v_term_command);
  string_init(argp_v_exec_wrapper);
  string_init(argp_v_no_db_flag_file);

  string_list_init(argp_v_script_dirs);
  string_list_init(argp_v_file_regexes);

  expand_tilde_string(SCRIPT_ROFI_DB_FLAG_FILE, argp_v_db_file);
  expand_tilde_string(SCRIPT_ROFI_XTERM_COMMAND, argp_v_term_command);
  expand_tilde_string(SCRIPT_ROFI_NO_DB_FLAG_FILE, argp_v_no_db_flag_file);

  // TODO default -- argp_v_script_dirs
}

void argp_cleanup()
{
  string_clear(argp_v_db_file);
  string_clear(argp_v_term_command);
  string_clear(argp_v_exec_wrapper);
  string_clear(argp_v_no_db_flag_file);

  string_list_clear(argp_v_script_dirs);
  string_list_clear(argp_v_file_regexes);
}


int argp_parse(const int argc, char* argv[])
{
    int opt;

    while ((opt = getopt(argc, argv, "?hpsePS:D:T:W:/:i")) != -1) {
        switch (opt) {
        case 's':
          argp_v_save = true;
          break;

        case 'e':
          argp_v_execute = true;
          break;

        case 'p':
          argp_v_print = true;
          break;

        case 'P':
          argp_v_dump_and_exit = true;
          break;

        case 'S': break; // TODO

        case 'D':
          break; // TODO

        case 'T': break; // TODO
          
        case 'W': break; // TODO
          
        case '/': break; // TODO

        case 'i':
          argp_v_ignorecase = true;
          break;

        case '?':
        case 'h':
            argp_print_usage(stdout);
            exit(EXIT_FAILURE);
            break;
        }
    }

    return 0;
}

void argp_print_usage(FILE* fp)
{
#define P(s, ...) fprintf(fp, s, ##__VA_ARGS__)
    P("It asks to select a script within SCRIPT_DIRS and execute "
      "it.\n");
    P("\n");
    P("(NO_DB_FLAG_FILE:  %s)\n", string_get_cstr(argp_v_no_db_flag_file));
    P("\n");
    P("-p : print selection\n");
    P("-s : save selection\n");
    P("-e : execute selection\n");
    P("-S SCRIPT_DIRS  (':'-separated list)\n");
    P("-D HIST_DB_FILE   : %s\n", string_get_cstr(argp_v_db_file));
    P("-T XTERM_COMMAND  : %s\n", string_get_cstr(argp_v_term_command));
    P("-P : Dump stored history/freqs and exit\n");
    P("-W : execute wrapper (like 'wine')\n");
    P("-/ : filename matching regex\n");
    P("-i : ignorecase\n");
    P("\n");
    P("Exiting.\n");
#undef P
}
