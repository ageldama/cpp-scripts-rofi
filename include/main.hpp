#ifndef MAIN_HPP
#define MAIN_HPP

#include "string_vector.hpp"
#include <span>

namespace SR {
void print_dump();
auto sorted_file_list() -> SR::string_vector;
void main2(const std::span<char*>& args);
}

#endif /* MAIN_HPP */
