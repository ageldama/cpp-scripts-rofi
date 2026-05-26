#include "str.hpp"
#include <sstream>
#include <string>

namespace SR::str {
SR::string_vector tokenize_commandline(const std::string& cmdline)
{
    std::stringstream ss(cmdline);
    std::string token;

    SR::string_vector results;
    while (ss >> token) {
        results.push_back(token);
    }

    return results;
}

void tokenize_cmd_and_prepend(
    SR::string_vector& target, const std::string& cmdline)
{
    const auto tokens = tokenize_commandline(cmdline);
    target.insert(target.begin(), tokens.begin(), tokens.end());
}

}
