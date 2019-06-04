#pragma once

#include <vector>

// 获取伪随机值，取值范围 [min_num, max_num)
int RandNum(int min_num, int max_num);

// 根据权重随机获取项
int GetRandIndex(const std::vector<int> &weight_list, int total_weight = 0);
