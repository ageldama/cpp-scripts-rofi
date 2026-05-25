#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "argp.h"
#include "db.h"

int main(int argc, char* argv[])
{
    int rc = 0;

    argp_init();
    atexit(argp_cleanup);
    rc = argp_parse(argc, argv);
    (void)rc;

    db_init();
    atexit(db_cleanup);

    exit(EXIT_SUCCESS);
}

#if 0
/*

--- TODOs ------------------------------------------------------------

* save/load db-file : dict(k:str/v:int) dict(k:str/v:[]int)

* rofi : select list, ask-yn

* execvp

* regex matching

* dirent find files


--- DONEs ------------------------------------------------------------

* arg-parsing


*/
#endif
