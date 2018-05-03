#include "RandomNumberGenerator.h"
#include "ServiceLocator.h"

RandomNumberGenerator::RandomNumberGenerator()
{
	m_mt = std::mt19937(m_rd());
}

RandomNumberGenerator::~RandomNumberGenerator()
{
}

int RandomNumberGenerator::GetIntWithinRange(int min, int max)
{
	std::uniform_int_distribution<int> dist(min, max);

	return dist(ServiceLocator::GetRandomNumberGenerator()->m_mt);
}