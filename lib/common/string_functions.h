#pragma once

#include <vector>
#include <string>
#include <sstream>

// 将一种内容， 比如是某种标识符，转换成字符串
#define MakeString(Identifier) #Identifier

// 按分隔符分解字符串，将分割好的字符串返回
std::vector<std::string> SplitString(const std::string &str, const char* delim);

// 格式化字符串
std::string StringFormat(const char* fmt, ...);
bool StringFormat2(char str[], int len, const char* fmt, ...);

// string转换成基础类型，返回是否转换成功
template<typename T>
bool StringToBaseType(const char *str, T &val)
{
	std::istringstream iss(str);
	if (iss >> val)
		return true;
	else
		return false;
}

// 基础类型转换成string，返回是否转换成功
template<typename T>
bool BaseTypeToString(T &val, std::string &str)
{
	std::ostringstream oss;
	if (oss << val) {
		str = oss.str();
		return true;
	}
	return false;
}
