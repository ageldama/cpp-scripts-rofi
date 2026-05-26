#include "str.hpp"
#include <iterator>
#include <sstream>
#include <string>

namespace SR::str {

SR::string_vector tokenize_commandline(const std::string& cmdline)
{
    std::stringstream iss(cmdline);
    return { std::istream_iterator<std::string> { iss },
        std::istream_iterator<std::string> {} };
}

void tokenize_cmd_and_prepend(
    SR::string_vector& target, const std::string& cmdline)
{
    const auto tokens = tokenize_commandline(cmdline);
    target.insert(target.begin(), tokens.begin(), tokens.end());
}

}
