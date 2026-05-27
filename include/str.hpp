#ifndef STR_HPP
#define STR_HPP

#include "string_vector.hpp"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <ranges>
#include <string>

namespace SR::str {

auto trim(const std::string& str) -> std::string;

auto tokenize_commandline(const std::string& cmdline)
    -> SR::string_vector;

void tokenize_cmd_and_prepend(
    SR::string_vector& target, const std::string& cmdline);

auto split_tokens(const std::string& s, char sep)
    -> SR::string_vector;

}

#endif /* STR_HPP */
