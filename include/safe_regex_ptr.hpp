#ifndef SAFE_REGEX_PTR_HPP
#define SAFE_REGEX_PTR_HPP

#include <memory>
#include <regex.h> // NOTE <regex> 너무 컴파일 느려서.

struct regex_deleter {
    void operator()(regex_t* r) const
    {
        if (r) {
            regfree(r);
            delete r;
        }
    }
};

// NOTE unique_ptr 쓰면, 이걸 쓰는 람다가 move_only_function여야
// 하고... C++23.
using safe_regex_ptr = std::shared_ptr<regex_t>;

safe_regex_ptr make_regex_ptr(const char* pattern)
{
    safe_regex_ptr ptr(new regex_t(), regex_deleter());

    if (regcomp(ptr.get(), pattern, REG_EXTENDED) != 0) {
        return nullptr;
    }

    return ptr;
}

#endif /* SAFE_REGEX_PTR_HPP */
