#include "utilities.hpp"
#include <iostream>
#include <random>

int next_status(const std::span<const int> statuses)
{
    if (statuses.empty()) 
    {
        std::cout << "Panic: empty statuses list\n" << std::flush;
        std::terminate();
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, statuses.size() - 1);

    const int randomIndex = distrib(gen);
    return statuses[randomIndex];
}
