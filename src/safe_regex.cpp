#include "safe_regex.hpp"

namespace SR::safe_regex {

void regex_deleter::operator()(regex_t* r) const
{
    if (r) {
        regfree(r);
        delete r;
    }
}

regex_ptr make_regex_ptr(const char* pattern)
{
    regex_ptr ptr(new regex_t(), regex_deleter());

    if (regcomp(ptr.get(), pattern, REG_EXTENDED) != 0) {
        return nullptr;
    }

    return ptr;
}

}
