#include "main.hpp"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

auto main(int argc, char** argv) -> int
{
    try {
        const auto args = std::span<char*>(argv, argc);
        main2(args);
        ::exit(EXIT_SUCCESS);
    } catch (std::exception& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
        ::exit(EXIT_FAILURE);
    }
}
