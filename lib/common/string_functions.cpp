#include "string_functions.h"
#include <cstring>

std::vector<std::string> SplitString(const std::string &str, const char *delim)
{
	std::vector<std::string> result;
	size_t delim_len = strlen(delim);
	std::string::size_type pos1 = 0;
	std::string::size_type pos2 = str.find(delim);

	while (pos2 != std::string::npos)
	{
		result.push_back(std::string(str, pos1, pos2 - pos1));

		pos1 = pos2 + delim_len;
		pos2 = str.find(delim, pos1);
	}

	if (pos1 != str.length())
	{
		result.push_back(std::string(str, pos1));
	}

	return result;
}
