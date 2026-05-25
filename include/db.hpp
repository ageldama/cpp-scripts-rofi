#ifndef DB_H
#define DB_H

#include <time.h>
#include <string>
#include <vector>
#include <map>

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

extern db_t db_v_db;

extern "C" {
 void db_init();
 void db_cleanup();

 bool db_save(const char* filename);
 bool db_load(const char* filename);
}


#endif /* DB_H */
