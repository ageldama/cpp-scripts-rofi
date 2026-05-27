#include "db.hpp"
#include <cstring>
#include <filesystem>

namespace SR::db {
db_t v_db;

void init() { }

void cleanup() { }

constexpr std::string_view db_magic = "C-SCRIPTS-ROFI-2.0";
constexpr std::size_t db_magic_len = db_magic.length();

auto save(const char* filename) -> bool
{
    FILE* fp = nullptr;
    fp = fopen(filename, "wb");
    if (nullptr == fp) {
        perror("fopen for write hist");
        return false;
    }

    fwrite(static_cast<const void*>(std::string(db_magic).c_str()),
        db_magic_len, 1, fp);

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

        // run_alt
        uint8_t run_alt_i = entry.run_alt ? 1 : 0;
        fwrite(&run_alt_i, sizeof(uint8_t), 1, fp);
    }

    fclose(fp);
    fp = nullptr;

    return true; // ok
}

void load(const char* filename)
{
    namespace fs = std::filesystem;

    if (!fs::exists(filename) || !fs::is_regular_file(filename)) {
        throw FileNotFoundException(filename);
    }

    if (fs::file_size(filename) < db_magic_len) {
        throw FileMagicInvalidException(filename);
    }

    FILE* fp = nullptr;
    fp = fopen(filename, "rb");
    if (nullptr == fp) {
        throw FileNotFoundException(filename);
    }

    // magic header
    char* magic_buf = static_cast<char*>(malloc(db_magic_len));
    fread(magic_buf, db_magic_len, 1, fp);
    bool magic_ok = ::strncmp(std::string(db_magic).c_str(),
                        magic_buf, db_magic_len)
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

        // run_alt
        uint8_t run_alt_i = 0;
        fread(&run_alt_i, sizeof(uint8_t), 1, fp);
        entry.run_alt = run_alt_i > 0 ? true : false;

        //
        v_db.insert_or_assign(cmd, entry);
    }

    fclose(fp);
}

auto get(const std::string& cmd)
    -> std::optional<std::reference_wrapper<db_entry>>
{
    auto it = v_db.find(cmd);
    if (it != v_db.end()) {
        return it->second;
    }
    return std::nullopt;
}

auto get_last_epoch(const std::string& cmd) -> time_t
{
    auto entry_opt = get(cmd);
    if (entry_opt) {
        const db_entry& entry_ref = entry_opt.value();
        return entry_ref.last_epoch;
    }
    return 0;
}

auto upd_last_epoch(const std::string& cmd) -> time_t
{
    time_t now = time(nullptr);

    auto entry_opt = get(cmd);
    if (entry_opt) {
        db_entry& entry_ref = entry_opt.value();
        entry_ref.last_epoch = now;
    } else {
        v_db[cmd] = db_entry {
            .last_epoch = now,
            .run_alt = false,
        };
    }

    return now;
}

auto set_run_alt(const std::string& cmd, const bool run_alt) -> bool
{
    auto entry_opt = get(cmd);
    if (entry_opt) {
        db_entry& entry_ref = entry_opt.value();
        entry_ref.run_alt = run_alt;
    } else {
        v_db[cmd] = db_entry {
            .last_epoch = 0,
            .run_alt = run_alt,
        };
    }
    return run_alt;
}

auto is_run_alt(const std::string& cmd) -> bool
{
    auto entry_opt = get(cmd);
    if (entry_opt) {
        const db_entry& entry_ref = entry_opt.value();
        return entry_ref.run_alt;
    }
    return false;
}

auto toggle_run_alt(const std::string& cmd) -> bool
{
    const auto curr = is_run_alt(cmd);
    return set_run_alt(cmd, !curr);
}

}
