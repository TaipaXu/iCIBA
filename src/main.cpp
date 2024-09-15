#include "core.hpp"

int main(int argc, char *argv[])
{
    constexpr const Core core;
    core.start(argc, argv);

    return EXIT_SUCCESS;
}
