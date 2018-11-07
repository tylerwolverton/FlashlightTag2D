#pragma once
#include <random>

class RandomNumberGenerator
{
public:
    RandomNumberGenerator();
    ~RandomNumberGenerator();

    static int GetIntWithinRange(int min, int max);

    std::random_device m_rd;
    std::mt19937 m_mt;
};

