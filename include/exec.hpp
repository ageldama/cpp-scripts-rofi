#ifndef EXEC_HPP
#define EXEC_HPP

#include "string_vector.hpp"
#include <string>

namespace SR::exec {
std::string execvp(const string_vector& cmdv);
}

#endif /* EXEC_HPP */
