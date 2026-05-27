#ifndef SAFE_REGEX_HPP
#define SAFE_REGEX_HPP

#include <memory>
#include <regex.h> // NOTE <regex> 너무 컴파일 느려서.

namespace SR::safe_regex {

struct regex_deleter {
    void operator()(regex_t* r) const;
};

// NOTE unique_ptr 쓰면, 이걸 쓰는 람다가 move_only_function여야
// 하고... C++23.
using regex_ptr = std::shared_ptr<regex_t>;

 regex_ptr make_regex_ptr(const char* pattern);

}

#endif /* SAFE_REGEX_HPP */
