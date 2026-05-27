#ifndef FILE_FIND_HPP
#define FILE_FIND_HPP

#include "string_vector.hpp"

#include <filesystem>
#include <functional>

namespace SR::file_find {

using directory_entry_predicate
    = std::function<bool(const std::filesystem::directory_entry&)>;

auto find_in_directories(const string_vector& dirs,
    const directory_entry_predicate& pred) -> SR::string_vector;

auto only_file(const std::filesystem::directory_entry& entry) -> bool;

auto make_only_file_and_regex_match(const SR::string_vector& regexes)
    -> directory_entry_predicate;
}

#endif /* FILE_FIND_HPP */
