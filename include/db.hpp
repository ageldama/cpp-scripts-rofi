#ifndef DB_HPP
#define DB_HPP

#include <map>
#include <string>
#include <time.h>
#include <vector>

namespace SR::db {

typedef enum {
    RUN_NORMAL = 0,
    RUN_IN_TERM = 1,
} run_type_t;

using run_count_t = uint32_t;

struct db_entry {
    time_t last_epoch;
    std::vector<run_count_t> run_type_counts;
};

using db_t = std::map<std::string, db_entry>;

extern db_t v_db;

extern void init();

extern void cleanup();

extern bool save(const char* filename);

extern bool load(const char* filename);

}

#endif /* DB_HPP */
