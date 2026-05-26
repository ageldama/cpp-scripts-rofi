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

    auto dirs = string_vector { "/home/aamadleg/.wine" };
    auto patterns = string_vector { "\\.lnk$" };

    auto files = file_find::find_in_directories(
        dirs, file_find::make_only_file_and_regex_match(patterns));

    auto opts = SR::rofi::rofi_common_opts {
        .prompt
        = "Select a script to run (Shift-Enter == run-in-terminal)",
        .ignorecase = true,
    };
    auto res = SR::rofi::select_list(opts, files); // FIXME

    // SR::rofi::ask_yn("Y/N???", true, "", "YYYYYY", "NNNNN");

    exit(EXIT_SUCCESS);
}

#if 0
/*

--- TODOs ------------------------------------------------------------

* rofi : select list

* execvp



--- DONEs ------------------------------------------------------------

* rofi : ask-yn

* arg-parsing
* save/load db-file : dict(k:str/v:int) dict(k:str/v:[]int)

* regex matching
* dirent find files



*/
#endif
