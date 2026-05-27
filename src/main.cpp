#include "main.hpp"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

auto main(int argc, char** argv) -> int
{
    using namespace std;
    using namespace SR;
    try {
        const auto args = span<char*>(argv, argc);
        main2(args);
        ::exit(EXIT_SUCCESS);
    } catch (exception& e) {
        cerr << "[ERROR] " << e.what() << endl;
        ::exit(EXIT_FAILURE);
    }
}
