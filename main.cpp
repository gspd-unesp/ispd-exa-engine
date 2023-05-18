#include <iostream>
#include <scheduler/round_robin.hpp>
#include <service/link.hpp>
#include <service/machine.hpp>
#include <service/master.hpp>
#include <simulator/timewarp.hpp>

/**
 * @brief Simulator entry point.
 */
int main(int argc, char **argv)
{
    --argc, ++argv;
    std::cout << "For the time being, use the `test` directory" << std::endl;
    return 0;
}
