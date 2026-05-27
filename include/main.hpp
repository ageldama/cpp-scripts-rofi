#ifndef MAIN_HPP
#define MAIN_HPP

#include "db.hpp"
#include "string_vector.hpp"
#include <optional>
#include <span>
#include <string>

auto ask_most_run_type(const std::string& cmd, bool alt)
    -> std::optional<SR::db::run_type_t>;

auto sorted_file_list() -> SR::string_vector;

void print_dump();

void main2(const std::span<char*>& args);

#endif /* MAIN_HPP */
