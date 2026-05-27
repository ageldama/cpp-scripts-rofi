#include <cstdio>
#include <cstdlib>
#include <unistd.h>

#include <string>

#include <filesystem>

#include "string_vector.hpp"

#include "argp.hpp"
#include "tildeexpand.hpp"
#include "str.hpp"

#define SCRIPT_ROFI_NO_DB_FLAG_FILE "~/.no-db-scripts-rofi"
#define SCRIPT_ROFI_DB_FLAG_FILE "~/.scripts-rofi.hist"
#define SCRIPT_ROFI_XTERM_COMMAND "x-terminal-emulator -e"
#define SCRIPT_ROFI_SCRIPT_DIRS                                      \
    "~/local/scripts:~/local/bin:~/.screenlayout:~/P/v3/bin"

namespace SR::argp {

bool v_print = false;
bool v_save = false;
bool v_execute = false;
SR::string_vector v_script_dirs;
std::string v_db_file;
std::string v_term_command;
bool v_dump_and_exit = false;
std::string v_exec_wrapper;
SR::string_vector v_file_regexes;
bool v_ignorecase = false;
std::string v_no_db_flag_file;

void init()
{
    SR::tilde::expand(SCRIPT_ROFI_DB_FLAG_FILE, v_db_file);

    v_term_command.assign(SCRIPT_ROFI_XTERM_COMMAND);

    SR::tilde::expand(SCRIPT_ROFI_NO_DB_FLAG_FILE, v_no_db_flag_file);

    set_script_dirs(SCRIPT_ROFI_SCRIPT_DIRS);
}

void cleanup() { }

int parse(const int argc, char* argv[])
{
    int opt;

    while ((opt = getopt(argc, argv, "?hpsePS:D:T:W:/:i")) != -1) {
        switch (opt) {
        case 's':
            v_save = true;
            break;

        case 'e':
            v_execute = true;
            break;

        case 'p':
            v_print = true;
            break;

        case 'P':
            v_dump_and_exit = true;
            break;

        case 'S':
            set_script_dirs(optarg);
            break;

        case 'D':
            SR::tilde::expand(optarg, v_db_file);
            break;

        case 'T':
            v_term_command.assign(optarg);
            break;

        case 'W':
            v_exec_wrapper.assign(optarg);
            break;

        case '/':
            set_file_regexes(optarg);
            break;

        case 'i':
            v_ignorecase = true;
            break;

        case '?':
        case 'h':
            print_usage(stdout);
            exit(EXIT_FAILURE);
            break;
        }
    }

    return 0;
}

void print_usage(FILE* fp)
{
#define P(s, ...) fprintf(fp, s __VA_OPT__(, ) __VA_ARGS__)
    P("It asks to select a script within SCRIPT_DIRS and execute "
      "it.\n", );
    P("\n", );
    P("(NO_DB_FLAG_FILE:  %s)\n", v_no_db_flag_file.c_str());
    P("\n", );
    P("-p : print selection\n", );
    P("-s : save selection\n", );
    P("-e : execute selection\n", );

    P("-S SCRIPT_DIRS  (':'-separated list)\n", );
    for (const auto& script_dir : v_script_dirs) {
        P("\t%s\n", script_dir.c_str());
    }

    P("-D HIST_DB_FILE   : %s\n", v_db_file.c_str());
    P("-T XTERM_COMMAND  : %s\n", v_term_command.c_str());
    P("-P : Dump stored history/freqs and exit\n", );
    P("-W : execute wrapper (like 'wine')\n", );

    P("-/ : filename matching regex\n", );
    for (const auto& file_regex : v_file_regexes) {
        P("\t%s\n", file_regex.c_str());
    }

    P("-i : ignorecase\n", );
    P("\n", );
    P("Exiting.\n", );
#undef P
}

void set_file_regexes(const char* arg)
{
  v_file_regexes = SR::str::split_tokens(std::string(arg), ':');
}

void set_script_dirs(const char* arg)
{
  v_script_dirs = SR::str::split_tokens(std::string(arg), ':');
  std::transform(v_script_dirs.begin(), v_script_dirs.end(),
                 v_script_dirs.begin(),
                 [](const std::string& s){
                   std::string expanded;
                   SR::tilde::expand(s.c_str(), expanded);
                   return expanded;
                 });
}

namespace fs = std::filesystem;

bool db_load_allowed()
{
  if (!v_db_file.empty()) {
        return false;
  }
  if (fs::exists(v_no_db_flag_file)){
        return false;
  }
  return true;
}

extern bool db_save_allowed() { return db_load_allowed() && v_save; }

}
