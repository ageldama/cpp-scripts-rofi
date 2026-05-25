#include "file_find.hpp"

#include <filesystem>
#include <string>



string_vector find_files_in_directories(const string_vector& dirs)
{
  string_vector results;

  for (const auto& dir : dirs) {
    for (const auto& entry : fs::recursive_directory_iterator(dir)) {
      // TODO
    }
  }

  return results;
}
