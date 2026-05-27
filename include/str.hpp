#ifndef STR_HPP
#define STR_HPP

#include "string_vector.hpp"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <ranges>
#include <string>

namespace SR::str {

std::string trim(const std::string& str);

SR::string_vector tokenize_commandline(const std::string& cmdline);

void tokenize_cmd_and_prepend(
    SR::string_vector& target, const std::string& cmdline);

SR::string_vector split_tokens(const std::string& s, const char sep);

}

#endif /* STR_HPP */
