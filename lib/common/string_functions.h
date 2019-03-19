#pragma once

#include <vector>
#include <string>

// 将一种内容， 比如是某种标识符，转换成字符串
#define MakeString(Identifier) (#Identifier)

// 按分隔符分解字符串，将分割好的字符串返回
std::vector<std::string> SplitString(const std::string &str, const char* delim);