#include "file_find.hpp"

#include "safe_regex.hpp"
#include <cassert>
#include <filesystem>
#include <functional>
#include <string>
#include <vector>

namespace SR::file_find {
namespace fs = std::filesystem;

bool only_file(const std::filesystem::directory_entry& entry)
{
    return fs::is_regular_file(entry);
}

directory_entry_predicate make_only_file_and_regex_match(
    const SR::string_vector regexes)
{
    using namespace SR::safe_regex;

    std::vector<regex_ptr> regex_ptrs;

    for (const auto& re : regexes) {
        auto regex_ptr = make_regex_ptr(re.c_str());
        assert(nullptr != regex_ptr);

        regex_ptrs.push_back(std::move(regex_ptr));
    }

    return
        [regex_ptrs](
            const std::filesystem::directory_entry& entry) -> bool {
            if (!fs::is_regular_file(entry))
                return false;
            std::string abs_path = entry.path().string();
            for (const auto& regex_ptr : regex_ptrs) {
                if (regexec(regex_ptr.get(), abs_path.c_str(), 0,
                        nullptr, 0)
                    == 0)
                    return true;
            }
            return false;
        };
}

SR::string_vector find_in_directories(const SR::string_vector& dirs,
    directory_entry_predicate entry_pred)
{
    SR::string_vector results;

    for (const auto& dir : dirs) {
        for (const auto& entry :
            fs::recursive_directory_iterator(dir)) {
            if (entry_pred(entry)) {
                std::string abs_path = entry.path().string();
                results.push_back(abs_path);
            }
        }
    }

    return results;
}
}
