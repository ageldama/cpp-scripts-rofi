#include "str.hpp"
#include <algorithm>
#include <sstream>
#include <string>

namespace SR::str {

const std::string whitespace = " \n\r\t\f\v";

auto ltrim(const std::string& s) -> std::string
{
    size_t start = s.find_first_not_of(whitespace);
    return (start == std::string::npos) ? "" : s.substr(start);
}

auto rtrim(const std::string& s) -> std::string
{
    size_t end = s.find_last_not_of(whitespace);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

auto trim(const std::string& str) -> std::string
{
    return rtrim(ltrim(str));
}

auto split_tokens(const std::string& s, const char sep)
    -> SR::string_vector
{
    std::stringstream ss(s);
    std::string token;
    SR::string_vector results;

    while (std::getline(ss, token, sep)) {
        results.emplace_back(token);
    }

    return results;
}

}
