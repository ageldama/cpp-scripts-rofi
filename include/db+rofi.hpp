#ifndef DB_PLUS_ROFI_HPP
#define DB_PLUS_ROFI_HPP

#include "db.hpp"
#include "rofi.hpp"

namespace SR::db {
  class db_run_alt_callbacks : public SR::rofi::run_alt_callbacks {
public:
    auto is_run_alt(const std::string& cmd)  -> bool override {
      return SR::db::is_run_alt(cmd);
    }

    auto toggle_run_alt(const std::string& cmd)  -> bool override {
      return SR::db::toggle_run_alt(cmd);
    }
};

}

#endif /* DB_PLUS_ROFI_HPP */
