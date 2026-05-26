#ifndef ARGP_HPP
#define ARGP_HPP

#include "string_vector.hpp"
#include <cstdio>

namespace SR::argp {
extern bool v_print;
extern bool v_save;
extern bool v_execute;
extern string_vector v_script_dirs;
extern std::string v_db_file;
extern std::string v_term_command;
extern bool v_dump_and_exit;
extern std::string v_exec_wrapper;
extern string_vector v_file_regexes;
extern bool v_ignorecase;
extern std::string v_no_db_flag_file;

extern void init();
extern void cleanup();
extern int parse(const int argc, char* argv[]);
extern void print_usage(FILE* fp);

extern void set_file_regexes(const char* arg);
extern void set_script_dirs(const char* arg);

extern bool db_load_allowed();
extern bool db_save_allowed();

}

#endif /* ARGP_HPP */
