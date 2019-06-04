#include "test_other_functions.h"
#include "common/other_functions.h"
#include <vector>
#include <map>

void test_GetRandIndex()
{
	std::vector<int> weight_list = {10, 20, 30, 10, 20};
	std::map<int, int> hit_map;

	for (int i = 0; i < 1000000; i++)
	{
		int index = GetRandIndex(weight_list);

		++hit_map[index];
	}

	printf("====hit info====\n");
	printf("index\ttimes\n");
	for (auto p : hit_map)
	{
		printf("%d\t%d\n", p.first, p.second);
	}
}
