#include "str.hpp"

namespace SR::str {

std::string trim(const std::string& str)
{
    auto is_space = [](unsigned char ch) { return std::isspace(ch); };

    auto view = str | std::views::drop_while(is_space)
        | std::views::reverse | std::views::drop_while(is_space)
        | std::views::reverse;

    return std::string(view.begin(), view.end());
}

}
