#ifndef DB__ROFI_HPP
#define DB__ROFI_HPP

#include "db.hpp"
#include "rofi.hpp"

namespace SR::db {
  class db_run_alt_callbacks : public SR::rofi::run_alt_callbacks {
public:
    bool is_run_alt(const std::string& cmd) override {
      return SR::db::is_run_alt(cmd);
    }

    bool toggle_run_alt(const std::string& cmd) override {
      return SR::db::toggle_run_alt(cmd);
    }
};

}

#endif /* DB__ROFI_HPP */
