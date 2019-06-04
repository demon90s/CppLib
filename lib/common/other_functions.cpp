#include "other_functions.h"
#include <cstdlib>
#include <random>

//------------------------------------------------------------------------------------
static std::random_device& RD()
{
	static std::random_device rd;
	return rd;
}

static std::mt19937& GEN()
{
	static std::mt19937 gen(RD()());
	return gen;
}

int RandNum(int min_num, int max_num)
{
	max_num -= 1;
	std::uniform_int_distribution<unsigned long> dist(min_num, max_num);
	return dist(GEN());
}
//------------------------------------------------------------------------------------

int GetRandIndex(const std::vector<int> &weight_list, int total_weight)
{
	if (total_weight == 0)
	{
		for (auto w : weight_list)
			total_weight += w;
	}

	if (total_weight <= 0)
		return -1;

	int rand_value = RandNum(0, total_weight);
	for (int i = 0; i < static_cast<int>(weight_list.size()); i++)
	{
		if (rand_value < weight_list[i])
			return i;

		rand_value -= weight_list[i];
	}

	return -1;
}
