#ifndef STR_HPP
#define STR_HPP

#include "string_vector.hpp"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <ranges>
#include <string>

namespace SR::str {

extern std::string trim(const std::string& str);

extern SR::string_vector tokenize_commandline(
    const std::string& cmdline);

extern void tokenize_cmd_and_prepend(
    SR::string_vector& target, const std::string& cmdline);

}

#endif /* STR_HPP */
