#include "db+rofi.hpp"
#include <iostream>
#include "argp.hpp"

namespace SR::db {

  auto db_run_alt_callbacks::is_run_alt(const std::string& cmd) -> bool
    {
        return SR::db::is_run_alt(cmd);
    }

  auto db_run_alt_callbacks::toggle_run_alt(const std::string& cmd) -> bool 
    {
        const auto res = SR::db::toggle_run_alt(cmd);

        if (SR::argp::db_save_allowed()) {
            const auto saved
                = SR::db::save(SR::argp::v_db_file.c_str());
            if (!saved) {
                std::cerr << "[IGNORE] save error: " + argp::v_db_file
                          << std::endl;
            }
        }

        return res;
    }

}

