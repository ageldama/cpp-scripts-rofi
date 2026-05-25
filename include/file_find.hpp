#ifndef FILE_FIND_HPP
#define FILE_FIND_HPP

#include "string_vector.hpp"

#include <functional>
#include <filesystem>


using directory_entry_pred = std::function<bool(const std::filesystem::directory_entry&)>;


extern string_vector find_files_in_directories(const string_vector& dirs, directory_entry_pred entry_pred);

extern "C" {
  bool only_file(const std::filesystem::directory_entry& entry);

  directory_entry_pred make_only_file_and_regex_match(const string_vector regexes);
}


#endif /* FILE_FIND_HPP */
