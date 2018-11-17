#pragma once
#include <random>

class RandomNumberGenerator
{
public:
    RandomNumberGenerator();
    ~RandomNumberGenerator();

    static int GetIntWithinRange(int min, int max);
    static float GetFloatWithinRange(float min, float max);

    std::random_device m_rd;
    std::mt19937 m_mt;
};

