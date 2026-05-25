#include <cstdio>
#include <cstdlib>
#include <unistd.h>

#include <string>
#include "string_vector.hpp"

#include "argp.hpp"
#include "tildeexpand.hpp"

bool argp_v_print = false;
bool argp_v_save = false;
bool argp_v_execute = false;
string_vector argp_v_script_dirs;
std::string argp_v_db_file;
std::string argp_v_term_command;
bool argp_v_dump_and_exit = false;
std::string argp_v_exec_wrapper;
string_vector argp_v_file_regexes;
bool argp_v_ignorecase = false;
std::string argp_v_no_db_flag_file;

void argp_init()
{
    expand_tilde(SCRIPT_ROFI_DB_FLAG_FILE, argp_v_db_file);

    expand_tilde(
        SCRIPT_ROFI_XTERM_COMMAND, argp_v_term_command);

    expand_tilde(
        SCRIPT_ROFI_NO_DB_FLAG_FILE, argp_v_no_db_flag_file);

    argp_set_script_dirs(SCRIPT_ROFI_SCRIPT_DIRS);
}

void argp_cleanup()
{
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

        case 'S':
            argp_set_script_dirs(optarg);
            break;

        case 'D':
            expand_tilde(optarg, argp_v_db_file);
            break;

        case 'T':
            expand_tilde(optarg, argp_v_term_command);
            break;

        case 'W':
            expand_tilde(optarg, argp_v_exec_wrapper);
            break;

        case '/':
            argp_set_file_regexes(optarg);
            break;

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
    P("(NO_DB_FLAG_FILE:  %s)\n",
      argp_v_no_db_flag_file.c_str());
    P("\n");
    P("-p : print selection\n");
    P("-s : save selection\n");
    P("-e : execute selection\n");

    P("-S SCRIPT_DIRS  (':'-separated list)\n");
    for (const auto& script_dir:argp_v_script_dirs)
        {
          P("\t%s\n", script_dir.c_str());
        }

    P("-D HIST_DB_FILE   : %s\n", argp_v_db_file.c_str());
    P("-T XTERM_COMMAND  : %s\n",
      argp_v_term_command.c_str());
    P("-P : Dump stored history/freqs and exit\n");
    P("-W : execute wrapper (like 'wine')\n");

    P("-/ : filename matching regex\n");
    for(const auto& file_regex:argp_v_file_regexes)
        {
          P("\t%s\n", file_regex.c_str());
        }

    P("-i : ignorecase\n");
    P("\n");
    P("Exiting.\n");
#undef P
}

void argp_set_file_regexes(const char* arg)
{
}

void argp_set_script_dirs(const char* arg)
{
}
