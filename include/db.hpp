#ifndef DB_HPP
#define DB_HPP

#include <cstdint>
#include <ctime>
#include <functional>
#include <map>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

namespace SR::db {

enum run_type_t {
    RUN_UNKNOWN = -1,
    RUN_NORMAL = 0,
    RUN_IN_TERM = 1,
};

using run_count_t = uint32_t;

struct db_entry {
    time_t last_epoch;
    std::vector<run_count_t> run_type_counts;
};

using db_t = std::map<std::string, db_entry>;

extern db_t v_db;

void init();

void cleanup();

bool save(const char* filename);

void load(const char* filename);

std::optional<std::reference_wrapper<db_entry>> get(
    const std::string& cmd);

time_t upd_last_epoch(const std::string& cmd);

time_t get_last_epoch(const std::string& cmd);

run_count_t incr_run_count(
    const std::string& cmd, const run_type_t run_type);

run_type_t get_most_run_type(
    const std::string& cmd, const run_type_t default_val);

class FileNotFoundException : public std::runtime_error {
public:
    FileNotFoundException(const std::string& filename)
        : std::runtime_error(filename)
    {
    }
};

class FileMagicInvalidException : public std::runtime_error {
public:
    FileMagicInvalidException(const std::string& filename)
        : std::runtime_error(filename)
    {
    }
};

}

#endif /* DB_HPP */
