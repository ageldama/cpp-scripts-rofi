#ifndef DB_HPP
#define DB_HPP

#include <cstdint>
#include <ctime>
#include <map>
#include <optional>
#include <stdexcept>
#include <string>

namespace SR::db {

struct db_entry {
    time_t last_epoch;
    bool run_alt;
};

using db_t = std::map<std::string, db_entry>;

extern db_t v_db;

void init();

void cleanup();

auto save(const char* filename) -> bool;

void load(const char* filename);

auto get(const std::string& cmd)
    -> std::optional<std::reference_wrapper<db_entry>>;

auto upd_last_epoch(const std::string& cmd) -> time_t;

auto get_last_epoch(const std::string& cmd) -> time_t;

auto set_run_alt(const std::string& cmd, bool run_alt) -> bool;

auto toggle_run_alt(const std::string& cmd) -> bool;

auto is_run_alt(const std::string& cmd) -> bool;

class FileNotFoundException : public std::runtime_error {
public:
    FileNotFoundException(const std::string& filename)
        : std::runtime_error("FileNotFoundException: " + filename)
    {
    }
};

class FileMagicInvalidException : public std::runtime_error {
public:
    FileMagicInvalidException(const std::string& filename)
        : std::runtime_error("FileMagicInvalidException: " + filename)
    {
    }
};

}

#endif /* DB_HPP */
