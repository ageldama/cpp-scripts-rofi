#include "db.hpp"

db_t db_v_db;

void db_init() { }

void db_cleanup() { }

bool db_save(const char* filename)
{
    FILE* fp = NULL;
    fp = fopen(filename, "wb");
    if (NULL == fp)
        return false;

    size_t tot = db_v_db.size();
    fwrite(static_cast<void*>(&tot), sizeof(tot), 1, fp);

    for (const auto& [cmd, entry] : db_v_db) {
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

bool db_load(const char* filename)
{
    FILE* fp = NULL;
    fp = fopen(filename, "rb");
    if (NULL == fp)
        return false;

    size_t tot_cmd = 0;
    fread(static_cast<void*>(&tot_cmd), sizeof(tot_cmd), 1, fp);

    db_v_db.clear();

    for (size_t i_cmd = 0; i_cmd < tot_cmd; i_cmd++) {
        // cmd_len, cmd
        size_t cmd_len = 0;
        fread(static_cast<void*>(&cmd_len), sizeof(cmd_len), 1, fp);

        char* cmd_buf = new char[cmd_len + 1];
        fread(static_cast<void*>(cmd_buf), cmd_len, 1, fp);
        std::string cmd(cmd_buf, cmd_len);
        delete cmd_buf;

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
        db_v_db.insert_or_assign(cmd, entry);
    }

    fclose(fp);
    return true; // ok
}
