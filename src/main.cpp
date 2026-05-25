#include <cstdio>
#include <cstdlib>
#include <unistd.h>

#include "argp.hpp"
#include "db.hpp"
#include "file_find.hpp"
#include "rofi.hpp"
#include "str.hpp"

#include <iostream>

using namespace std;
using namespace SR;

int main(int argc, char* argv[])
{
    int rc = 0;

    argp::init();
    atexit(argp::cleanup);
    rc = argp::parse(argc, argv);
    (void)rc;

    db::init();
    atexit(db::cleanup);

    /*
    db_load(argp_v_db_file.c_str());
    db_save(argp_v_db_file.c_str());
    */

    /*
    auto dirs = string_vector { "/home/aamadleg/.wine" };
    auto patterns = string_vector { "\\.lnk$" };

    auto files = file_find::find_in_directories(
        dirs, file_find::make_only_file_and_regex_match(patterns));

    for (const auto& file : files) {
        std::cout << file << std::endl;
    }
    */

    SR::string_vector cmdv = {
        "rofi",
        "-theme-str",
        "window {width: 200px; height: 150px;}",
        "-dmenu",
        "-p",
        "???",
        "-sep",
        "\\n",
        "-eh",
        "2",
        "-markup-rows",
        "-format",
        "i",
    };

    auto res = SR::rofi::run_rofi(cmdv, [](const int fd) {
        SR::rofi::pipe_write(
            fd, "<span size='x-large' weight='heavy'>Yes</span>\n");
        SR::rofi::pipe_write(
            fd, "<span size='x-large' weight='heavy'>No</span>\n");
    });

    bool run_ok = false;
    int run_exitcode = 0;
    std::string run_stdout;
    std::tie(run_ok, run_exitcode, run_stdout) = res;

    std::string run_stdout_trimmed = str::trim(run_stdout);

    std::cout << "RUN_OK: " << run_ok << std::endl;
    std::cout << "RUN-EXITCODE: " << run_exitcode << std::endl;
    std::cout << "RUN-STDOUT: [" << run_stdout_trimmed << "]"
              << std::endl;

    exit(EXIT_SUCCESS);
}

#if 0
/*

--- TODOs ------------------------------------------------------------

* rofi : select list, ask-yn

* execvp



--- DONEs ------------------------------------------------------------

* arg-parsing
* save/load db-file : dict(k:str/v:int) dict(k:str/v:[]int)

* regex matching
* dirent find files



*/
#endif
