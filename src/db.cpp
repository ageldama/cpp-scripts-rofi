#include "db.hpp"
#include <algorithm>
#include <cstring>
#include <filesystem>
#include <functional>
#include <string_view>

namespace SR::db {
db_t v_db;

void init() { }

void cleanup() { }

constexpr std::string_view DB_MAGIC = "C-SCRIPTS-ROFI";
constexpr std::size_t DB_MAGIC_LEN = DB_MAGIC.length();

bool save(const char* filename)
{
    FILE* fp = NULL;
    fp = fopen(filename, "wb");
    if (NULL == fp) {
        perror("fopen for write hist");
        return false;
    }

    fwrite(static_cast<const void*>(std::string(DB_MAGIC).c_str()),
        DB_MAGIC_LEN, 1, fp);

    size_t tot = v_db.size();
    fwrite(static_cast<void*>(&tot), sizeof(tot), 1, fp);

    for (const auto& [cmd, entry] : v_db) {
        // [cmd_len, cmd]
        size_t cmd_len = cmd.size();
        fwrite(static_cast<void*>(&cmd_len), sizeof(cmd_len), 1, fp);
        fwrite(
            static_cast<const void*>(cmd.c_str()), cmd.size(), 1, fp);

        // last_epoch
        fwrite(static_cast<const void*>(&(entry.last_epoch)),
            sizeof(time_t), 1, fp);

        // run_type_counts
        size_t tot_run_types = entry.run_type_counts.size();
        fwrite(static_cast<void*>(&tot_run_types), sizeof(size_t), 1,
            fp);

        for (const auto& run_type_count : entry.run_type_counts) {
            fwrite(static_cast<const void*>(&run_type_count),
                sizeof(run_count_t), 1, fp);
        }
    }

    fclose(fp);
    fp = NULL;

    return true; // ok
}

void load(const char* filename)
{
    namespace fs = std::filesystem;

    if (!fs::exists(filename) || !fs::is_regular_file(filename)) {
        throw FileNotFoundException(filename);
    }

    if (fs::file_size(filename) < DB_MAGIC_LEN) {
        throw FileMagicInvalidException(filename);
    }

    FILE* fp = NULL;
    fp = fopen(filename, "rb");
    if (NULL == fp) {
        throw FileNotFoundException(filename);
    }

    // magic header
    char* magic_buf = static_cast<char*>(malloc(DB_MAGIC_LEN));
    fread(magic_buf, DB_MAGIC_LEN, 1, fp);
    bool magic_ok = ::strncmp(std::string(DB_MAGIC).c_str(),
                        magic_buf, DB_MAGIC_LEN)
        == 0;

    free(static_cast<void*>(magic_buf));

    if (!magic_ok) {
        fclose(fp);
        fp = nullptr;
        throw FileMagicInvalidException(filename);
    }

    // total count
    size_t tot_cmd = 0;
    fread(static_cast<void*>(&tot_cmd), sizeof(tot_cmd), 1, fp);

    v_db.clear();

    for (size_t i_cmd = 0; i_cmd < tot_cmd; i_cmd++) {
        // cmd_len, cmd
        size_t cmd_len = 0;
        fread(static_cast<void*>(&cmd_len), sizeof(cmd_len), 1, fp);

        char* cmd_buf = static_cast<char*>(malloc(cmd_len + 1));
        fread(static_cast<void*>(cmd_buf), cmd_len, 1, fp);
        std::string cmd(cmd_buf, cmd_len);
        free(cmd_buf);

        //
        db_entry entry;

        // last_epoch
        time_t epoch = 0;
        fread(static_cast<time_t*>(&epoch), sizeof(epoch), 1, fp);
        entry.last_epoch = epoch;

        // run_type_counts
        size_t tot_run_type_counts = 0;
        fread(static_cast<void*>(&tot_run_type_counts),
            sizeof(tot_run_type_counts), 1, fp);

        for (size_t i_run_type_count = 0;
             i_run_type_count < tot_run_type_counts;
             i_run_type_count++) {
            run_count_t run_count = 0;
            fread(static_cast<void*>(&run_count), sizeof(run_count),
                1, fp);
            entry.run_type_counts.push_back(run_count);
        }

        //
        v_db.insert_or_assign(cmd, entry);
    }

    fclose(fp);
}

std::optional<std::reference_wrapper<db_entry>> get(
    const std::string& cmd)
{
    auto it = v_db.find(cmd);
    if (it != v_db.end()) {
        return it->second;
    }
    return std::nullopt;
}

time_t get_last_epoch(const std::string& cmd)
{
    auto entry_ = get(cmd);
    if (entry_) {
        const db_entry& entry_ref = entry_.value();
        return entry_ref.last_epoch;
    }
    return 0;
}

time_t upd_last_epoch(const std::string& cmd)
{
    time_t now = time(nullptr);

    auto entry_ = get(cmd);
    if (entry_) {
        db_entry& entry_ref = entry_.value();
        entry_ref.last_epoch = now;
    } else {
        v_db[cmd] = db_entry {
            .last_epoch = now,
            .run_type_counts = std::vector<run_count_t> {},
        };
    }

    return now;
}

run_type_t get_most_run_type(
    const std::string& cmd, const run_type_t default_val)
{
    auto entry_ = get(cmd);
    if (entry_) {
        const db_entry& entry_ref = entry_.value();
        const auto& counts = entry_ref.run_type_counts;
        if (counts.empty())
            return default_val;

        const auto it
            = std::max_element(counts.begin(), counts.end());
        if (counts.end() == it)
            return default_val;

        const int idx = std::distance(counts.begin(), it);
        return static_cast<run_type_t>(idx);
    }
    return default_val;
}

run_count_t incr_run_count(
    const std::string& cmd, const run_type_t run_type)
{
    auto entry_ = get(cmd);

    if (!entry_) {
        auto entry = db_entry {
            .last_epoch = 0,
            .run_type_counts = std::vector<run_count_t> {},
        };
        v_db[cmd] = entry;
        entry_ = entry;
    }

    db_entry& entry = entry_.value();
    auto size = static_cast<std::vector<run_count_t>::size_type>(
        run_type + 1);
    if (entry.run_type_counts.size() < size) {
        entry.run_type_counts.resize(size, 0);
    }

    auto new_count = ++entry.run_type_counts[run_type];
    return new_count;
}

}
