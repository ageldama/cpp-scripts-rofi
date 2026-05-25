#include "db.h"

string_time_dict_t db_v_cmd_last_epochs;
string_run_types_array_dict_t db_v_cmd_run_types;

void db_init()
{
    string_time_dict_init(db_v_cmd_last_epochs);
    string_run_types_array_dict_init(db_v_cmd_run_types);
}

void db_cleanup()
{
    string_time_dict_clear(db_v_cmd_last_epochs);
    string_run_types_array_dict_clear(db_v_cmd_run_types);
}

bool db_save(const char* filename)
{
    FILE* fp = NULL;

    fp = fopen(filename, "wb");
    if (NULL == fp)
        return false;

    // cmd-last-epochs
    {
        size_t tot = string_time_dict_size(db_v_cmd_last_epochs);
        fwrite(&tot, sizeof(tot), 1, fp);

        string_time_dict_it_t it;
        for (string_time_dict_it(it, db_v_cmd_last_epochs);
             !string_time_dict_end_p(it); string_time_dict_next(it)) {
            //
            const struct string_time_dict_pair_s* pair
                = string_time_dict_cref(it);
            size_t keylen = strlen(string_get_cstr(pair->key));
            fwrite(&keylen, sizeof(size_t), 1, fp);
            fwrite(string_get_cstr(pair->key), keylen, 1, fp);
            time_t epoch = pair->value;
            fwrite(&epoch, sizeof(time_t), 1, fp);
        }
    }

    // cmd-run-types
    {
        size_t tot
            = string_run_types_array_dict_size(db_v_cmd_run_types);
        fwrite(&tot, sizeof(tot), 1, fp);

        string_run_types_array_dict_it_t it;
        for (string_run_types_array_dict_it(it, db_v_cmd_run_types);
             !string_run_types_array_dict_end_p(it);
             string_run_types_array_dict_next(it)) {
            (void)it; // TODO
        }
    }

    // closing
    fclose(fp);
    fp = NULL;

    return true; // ok
}

bool db_load(const char* filename)
{
    // TODO
    string_time_dict_reset(db_v_cmd_last_epochs);
    string_run_types_array_dict_reset(db_v_cmd_run_types);

    return true; // ok
}
