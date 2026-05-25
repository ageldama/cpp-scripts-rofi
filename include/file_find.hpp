#ifndef FILE_FIND_HPP
#define FILE_FIND_HPP

#include "string_vector.hpp"

extern "C" {
  string_vector find_files_in_directories(const string_vector& dirs);
}


#endif /* FILE_FIND_HPP */
