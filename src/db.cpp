#include "db.hpp"

db_t db_v_db;

void db_init()
{
}

void db_cleanup()
{
}

bool db_save(const char* filename)
{
  FILE *fp = NULL;
  fp = fopen(filename, "wb");
  if (NULL == fp) return false;

  size_t tot = db_v_db.size();
  fwrite(static_cast<void*>(&tot), sizeof(tot), 1, fp);

  for (const auto& [cmd, entry] : db_v_db)
    {
      // [cmd_len, cmd]
      size_t cmd_len = cmd.size();
      fwrite(static_cast<void*>(&cmd_len), sizeof(cmd_len), 1, fp);
      fwrite(static_cast<const void*>(cmd.c_str()),
             cmd.size(), 1, fp);

      // last_epoch
      fwrite(static_cast<const void*>(&(entry.last_epoch)),
             sizeof(time_t), 1, fp);

      // run_type_counts
      size_t tot_run_types = entry.run_type_counts.size();
      fwrite(static_cast<void*>(&tot_run_types),
             sizeof(size_t), 1, fp);

      for (const auto& run_type_count: entry.run_type_counts) {
        fwrite(static_cast<const void*>(&run_type_count),
               sizeof(run_count_t), 1, fp);
      }
    }

  fclose(fp); fp=NULL;

  return true; // ok
}

bool db_load(const char* filename)
{
    // TODO
    return true; // ok
}
