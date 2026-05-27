#ifndef EXEC_HPP
#define EXEC_HPP

#include "string_vector.hpp"
#include <string>

namespace SR::exec {
auto execvp(const string_vector& cmdv) -> std::string;
}

#endif /* EXEC_HPP */
