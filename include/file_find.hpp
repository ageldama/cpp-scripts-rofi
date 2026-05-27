#ifndef FILE_FIND_HPP
#define FILE_FIND_HPP

#include "string_vector.hpp"

#include <filesystem>
#include <functional>

namespace SR::file_find {

using directory_entry_predicate
    = std::function<bool(const std::filesystem::directory_entry&)>;

SR::string_vector find_in_directories(
    const string_vector& dirs, const directory_entry_predicate& pred);

bool only_file(const std::filesystem::directory_entry& entry);

directory_entry_predicate make_only_file_and_regex_match(
    const SR::string_vector regexes);
}

#endif /* FILE_FIND_HPP */
