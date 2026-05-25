#include <cstdio>
#include <cstdlib>
#include <unistd.h>

#include "argp.hpp"
#include "db.hpp"
#include "file_find.hpp"

#include <iostream>

int main(int argc, char* argv[])
{
    int rc = 0;

    argp_init();
    atexit(argp_cleanup);
    rc = argp_parse(argc, argv);
    (void)rc;

    db_init();
    atexit(db_cleanup);

    auto dirs = string_vector { "/home/aamadleg/.wine" };
    auto patterns = string_vector { "\\.lnk$" };

    auto files = find_files_in_directories(
        dirs, make_only_file_and_regex_match(patterns));

    for (const auto& file : files) {
        std::cout << file << std::endl;
    }

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
